#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"

static FILE *f;

static void in_normal_mode(struct fsm *myfsm);
static void entering_slash_detected(struct fsm *myfsm);
static void in_line_comment(struct fsm *myfsm);
static void in_block_comment(struct fsm *myfsm);
static void exit_star_detected(struct fsm *myfsm);

static void in_normal_mode(struct fsm *myfsm)
{
	char next_char = fgetc(f);

	switch (next_char) {
	case EOF :
		FSM_STOP(myfsm);
	case '/' :
		FSM_NEXT(myfsm, entering_slash_detected);
	default :
		printf("%c", next_char);
		FSM_NEXT(myfsm, in_normal_mode);
	}
}


static void entering_slash_detected(struct fsm *myfsm)
{
	char next_char = fgetc(f);

	switch (next_char) {
	case EOF :
		FSM_STOP(myfsm);
	case '/' :
		FSM_NEXT(myfsm, in_line_comment);
	case '*' :
		FSM_NEXT(myfsm, in_block_comment);
	default :
		printf("/%c", next_char);
	}	FSM_NEXT(myfsm, in_normal_mode);
}


static void in_line_comment(struct fsm *myfsm)
{
	char next_char = fgetc(f);

	switch (next_char) {
	case EOF :
		FSM_STOP(myfsm);
	case '\n' :
	case '\r' :
		printf("%c", next_char);
		FSM_NEXT(myfsm, in_normal_mode);
	default :
		FSM_NEXT(myfsm, in_line_comment);
	}
}


static void in_block_comment(struct fsm *myfsm)
{
	char next_char = fgetc(f);

	switch (next_char) {
	case EOF :
		FSM_STOP(myfsm);
	case '*' :
		FSM_NEXT(myfsm, exit_star_detected);
	default :
		FSM_NEXT(myfsm, in_block_comment);
	}
}


static void exit_star_detected(struct fsm *myfsm)
{
	char next_char = fgetc(f);

	switch (next_char) {
	case EOF :
		FSM_STOP(myfsm);
	case '*' :
		FSM_NEXT(myfsm, exit_star_detected);
	case '/' :
		FSM_NEXT(myfsm, in_normal_mode);
	default :
		FSM_NEXT(myfsm, in_block_comment);
	}
}


int main(int argc, char *argv[])
{
	FSM_DECLARE(parser);

	if (argc < 2)
		return 1;

	f = fopen(argv[1], "r");

	if (NULL == f)
		return 1;

	FSM_RUN(&parser, in_normal_mode);
	fclose(f);

	return 0;
}



