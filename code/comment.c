#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fsm.h"
#include "comment.h"

#define TU_MAX_LEN 2000000u

#define TEST_COMMENT
//#define DEBUG_COMMENT

struct parser {
	FILE *f;
	char *out_buf;
	unsigned int size;
};


FSM_STATE(in_normal_mode,          myfsm);
FSM_STATE(entering_slash_detected, myfsm);
FSM_STATE(in_line_comment,         myfsm);
FSM_STATE(in_block_comment,        myfsm);
FSM_STATE(exit_star_detected,      myfsm);

#ifdef DEBUG_COMMENT

#define LOG_STATE(_PARSER_) do {                                 \
	printf("\nEntering %s (line %d)\n", __func__, __LINE__); \
} while(0)

#else

#define LOG_STATE

#endif


static inline void stack(struct parser *my_parser, char c)
{
	if (my_parser->size < TU_MAX_LEN)
		my_parser->out_buf[my_parser->size++] = c;
	else
		exit(EXIT_FAILURE);
}

FSM_STATE(in_normal_mode, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char c = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (c) {
		case EOF : FSM_STOP(myfsm);
		case '/' : FSM_NEXT(myfsm, entering_slash_detected);
		default  : stack(my_parser, c);
			   FSM_NEXT(myfsm, in_normal_mode);
	}
}


FSM_STATE(entering_slash_detected, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char c = fgetc(my_parser->f);

	LOG_STATE(my_parser);
	switch (c) {
		case EOF : FSM_STOP(myfsm);
		case '/' : FSM_NEXT(myfsm, in_line_comment);
		case '*' : FSM_NEXT(myfsm, in_block_comment);
		default  : stack(my_parser, '/');
			   stack(my_parser, c);
	}	FSM_NEXT(myfsm, in_normal_mode);
}


FSM_STATE(in_line_comment, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char c = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (c) {
		case EOF  : FSM_STOP(myfsm);
		case '\r' : stack(my_parser, '\n');
			    FSM_NEXT(myfsm, in_normal_mode);
		case '\n' : stack(my_parser, c);
			    FSM_NEXT(myfsm, in_normal_mode);
		default   : FSM_NEXT(myfsm, in_line_comment);
	}
}


FSM_STATE(in_block_comment, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char c = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (c) {
		case EOF : FSM_STOP(myfsm);
		case '*' : FSM_NEXT(myfsm, exit_star_detected);
		default  : FSM_NEXT(myfsm, in_block_comment);
	}
}


FSM_STATE(exit_star_detected, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char c = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (c) {
		case EOF : FSM_STOP(myfsm);
		case '*' : FSM_NEXT(myfsm, exit_star_detected);
		case '/' : FSM_NEXT(myfsm, in_normal_mode);
		default  : FSM_NEXT(myfsm, in_block_comment);
	}
}

unsigned int comment(FILE *f, char *out_buf)
{
	struct parser my_parser = {f, out_buf, 0};
	FSM_DECLARE(comment);

	comment.priv = (void *)(&my_parser);
	FSM_RUN(&comment, in_normal_mode);

	return my_parser.size;
}

#ifdef TEST_COMMENT

int main(int argc, char *argv[])
{
	FILE *f;
	unsigned int i, ret;
	char my_buf[TU_MAX_LEN];

	if (argc < 2)
		return 1;

	f = fopen(argv[1], "r");

	if (NULL == f)
		return 1;

	ret = comment(f, my_buf);

	fclose(f);

	//printf("*********************************************************\n");
	printf("%s : %d\n", argv[1], ret);

	for (i = 0 ; i < ret ; i++)
		printf("%c", my_buf[i]);
	printf("\n");


	return 0;
}

#endif
