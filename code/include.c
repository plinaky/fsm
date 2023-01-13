#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"

static FILE *f;


#define PAT_MAX_SIZE 32u
struct pattern {
	char str [PAT_MAX_SIZE];
	unsigned char len;
	unsigned char pos;
};

static struct pattern inc_pattern = {
	"include\0",
	7,
	0
};

static void in_normal_mode(struct fsm *myfsm);
static void entering_hash_detected(struct fsm *myfsm);
static void detect_pattern(struct fsm *myfsm);
static void include_detected(struct fsm *myfsm);
static void skip_util_eol(struct fsm *myfsm);

static void in_normal_mode(struct fsm *myfsm)
{
	char next_char = fgetc(f);

	switch (next_char) {
		case EOF  : FSM_STOP(myfsm);
		case ' '  :
		case '\t' :
		case '\n' :
		case '\r' : FSM_NEXT(myfsm, in_normal_mode);
		case '#'  : FSM_NEXT(myfsm, entering_hash_detected);
		default   : FSM_NEXT(myfsm, skip_util_eol);
	}
}

static void entering_hash_detected(struct fsm *myfsm)
{
	char next_char = fgetc(f);

	switch (next_char) {
		case EOF  : FSM_STOP(myfsm);
		case ' '  :
		case '\t' : FSM_NEXT(myfsm, entering_hash_detected);
		case '\n' :
		case '\r' : FSM_NEXT(myfsm, in_normal_mode);
		case 'i'  : myfsm->data = (void *)&inc_pattern;
			    FSM_NEXT(myfsm, detect_pattern);
		default   : FSM_NEXT(myfsm, skip_util_eol);
	}
}

static void detect_pattern(struct fsm *myfsm)
{
	struct pattern *pat = (struct pattern *)myfsm->data;
	char next_char = fgetc(f);

	pat->pos++;
	if (next_char == pat->str[pat->pos]) {
		if (pat->len - 1 == pat->pos)
			FSM_NEXT(myfsm, include_detected);
		else 
			FSM_NEXT(myfsm, detect_pattern);
	}

	switch (next_char) {
		case EOF  : FSM_STOP(myfsm);
		case '\n' :
		case '\r' : FSM_NEXT(myfsm, in_normal_mode);
		default   : FSM_NEXT(myfsm, skip_util_eol);
	}
}

static void include_detected(struct fsm *myfsm)
{
	struct pattern *pat = (struct pattern *)myfsm->data;
	char next_char = fgetc(f);

	pat->pos = 0;
	switch (next_char) {
		case EOF  : FSM_STOP(myfsm);
		case '\n' :
		case '\r' : printf("%c", next_char);
			    FSM_NEXT(myfsm, in_normal_mode);
		default   : printf("%c", next_char);
			    FSM_NEXT(myfsm, include_detected);
	}
}

static void skip_util_eol(struct fsm *myfsm)
{
	char next_char = fgetc(f);

	switch (next_char) {
		case EOF  : FSM_STOP(myfsm);
		case '\n' :
		case '\r' : FSM_NEXT(myfsm, in_normal_mode);
		default   : FSM_NEXT(myfsm, skip_util_eol);
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

