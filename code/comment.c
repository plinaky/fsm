#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"
#include "comment.h"

#define TEST_COMMENT
//#define DEBUG_COMMENT

struct parser {
	FILE *f;
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


FSM_STATE(in_normal_mode, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (mychar) {
		case EOF : FSM_STOP(myfsm);
		case '/' : FSM_NEXT(myfsm, entering_slash_detected);
		default  : printf("%c", mychar);
			   FSM_NEXT(myfsm, in_normal_mode);
	}
}


FSM_STATE(entering_slash_detected, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (mychar) {
		case EOF : FSM_STOP(myfsm);
		case '/' : FSM_NEXT(myfsm, in_line_comment);
		case '*' : FSM_NEXT(myfsm, in_block_comment);
		default : printf("/%c", mychar);
	}	FSM_NEXT(myfsm, in_normal_mode);
}


FSM_STATE(in_line_comment, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (mychar) {
		case EOF  : FSM_STOP(myfsm);
		case '\n' :
		case '\r' : printf("%c", mychar);
			    FSM_NEXT(myfsm, in_normal_mode);
		default   : FSM_NEXT(myfsm, in_line_comment);
	}
}


FSM_STATE(in_block_comment, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (mychar) {
		case EOF : FSM_STOP(myfsm);
		case '*' : FSM_NEXT(myfsm, exit_star_detected);
		default  : FSM_NEXT(myfsm, in_block_comment);
	}
}


FSM_STATE(exit_star_detected, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (mychar) {
		case EOF : FSM_STOP(myfsm);
		case '*' : FSM_NEXT(myfsm, exit_star_detected);
		case '/' : FSM_NEXT(myfsm, in_normal_mode);
		default  : FSM_NEXT(myfsm, in_block_comment);
	}
}

void comment(FILE *f)
{
	struct parser my_parser = {f};
	FSM_DECLARE(comment);

	comment.priv = (void *)(&my_parser);
	FSM_RUN(&comment, in_normal_mode);
}

#ifdef TEST_COMMENT

int main(int argc, char *argv[])
{
	FILE *f;

	if (argc < 2)
		return 1;

	f = fopen(argv[1], "r");

	if (NULL == f)
		return 1;

	comment(f);

	fclose(f);

	return 0;
}

#endif
