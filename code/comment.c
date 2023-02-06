#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fsm.h"
#include "list.h"
#include "comment.h"

#define TU_MAX_LEN 2000000u

#define TEST_COMMENT
//#define DEBUG_COMMENT

struct parser {
	FILE *f;
	char *out_buf;
	unsigned int size;
	FSM_DECLARE(operator);
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


static inline void stack(struct parser *myparser, char c)
{
	if (myparser->size < TU_MAX_LEN)
		myparser->out_buf[myparser->size++] = c;
	else
		exit(EXIT_FAILURE);
}

FSM_STATE(in_normal_mode, myfsm)
{
	struct parser *myparser = container_of(myfsm, struct parser, operator);
	char c = fgetc(myparser->f);

	LOG_STATE(myparser);

	switch (c) {
		case EOF : FSM_STOP(myfsm);
		case '/' : FSM_NEXT(myfsm, entering_slash_detected);
		default  : stack(myparser, c);
			   FSM_NEXT(myfsm, in_normal_mode);
	}
}


FSM_STATE(entering_slash_detected, myfsm)
{
	struct parser *myparser = container_of(myfsm, struct parser, operator);
	char c = fgetc(myparser->f);

	LOG_STATE(myparser);
	switch (c) {
		case EOF : FSM_STOP(myfsm);
		case '/' : FSM_NEXT(myfsm, in_line_comment);
		case '*' : FSM_NEXT(myfsm, in_block_comment);
		default  : stack(myparser, '/');
			   stack(myparser, c);
	}	FSM_NEXT(myfsm, in_normal_mode);
}


FSM_STATE(in_line_comment, myfsm)
{
	struct parser *myparser = container_of(myfsm, struct parser, operator);
	char c = fgetc(myparser->f);

	LOG_STATE(myparser);

	switch (c) {
		case EOF  : FSM_STOP(myfsm);
		case '\r' : stack(myparser, '\n');
			    FSM_NEXT(myfsm, in_normal_mode);
		case '\n' : stack(myparser, c);
			    FSM_NEXT(myfsm, in_normal_mode);
		default   : FSM_NEXT(myfsm, in_line_comment);
	}
}


FSM_STATE(in_block_comment, myfsm)
{
	struct parser *myparser = container_of(myfsm, struct parser, operator);
	char c = fgetc(myparser->f);

	LOG_STATE(myparser);

	switch (c) {
		case EOF : FSM_STOP(myfsm);
		case '*' : FSM_NEXT(myfsm, exit_star_detected);
		default  : FSM_NEXT(myfsm, in_block_comment);
	}
}


FSM_STATE(exit_star_detected, myfsm)
{
	struct parser *myparser = container_of(myfsm, struct parser, operator);
	char c = fgetc(myparser->f);

	LOG_STATE(myparser);

	switch (c) {
		case EOF : FSM_STOP(myfsm);
		case '*' : FSM_NEXT(myfsm, exit_star_detected);
		case '/' : FSM_NEXT(myfsm, in_normal_mode);
		default  : FSM_NEXT(myfsm, in_block_comment);
	}
}

unsigned int comment(FILE *f, char *out_buf)
{
	struct parser myparser = {f, out_buf, 0};

	FSM_RUN(&myparser.operator, in_normal_mode);

	return myparser.size;
}

#ifdef TEST_COMMENT

int main(int argc, char *argv[])
{
	FILE *f;
	unsigned int i, ret;
	char mybuf[TU_MAX_LEN];

	if (argc < 2)
		return 1;

	f = fopen(argv[1], "r");

	if (NULL == f)
		return 1;

	ret = comment(f, mybuf);

	fclose(f);

	printf("%s : %d\n", argv[1], ret);

	for (i = 0 ; i < ret ; i++)
		printf("%c", mybuf[i]);
	printf("\n");


	return 0;
}

#endif
