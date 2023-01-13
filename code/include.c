#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"

#define PAT_MAX_SIZE 32u

struct pattern {
	char str [PAT_MAX_SIZE];
	unsigned char len;
	unsigned char pos;
};

struct inc_parser {
	FILE *f;	
	char directive[PAT_MAX_SIZE];
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

static FILE *f;

unsigned int inc_parse(FILE *infile)
{
	struct inc_parser my_inc_parser = {
		infile,
		"\0"
	};

	if (NULL == f)
		return 1;
	
	FSM_DECLARE(parser);
	parser->priv = (void *)(& my_inc_parser);
	FSM_RUN(&parser, in_normal_mode);
}


static void in_normal_mode(struct fsm *myfsm)
{
	char mychar;
	struct inc_parser *my_inc_parser;

	my_inc_parser = (struct inc_parser *)(myfsm->priv);
	mychar = fgetc(my_inc_parser->f);

#ifdef DEBUG_INCLUDE
	printf("%s : %c\n", __func__, mychar);
#endif

	switch (mychar) {
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
	char mychar;
	struct inc_parser *my_inc_parser;

	my_inc_parser = (struct inc_parser *)(myfsm->priv);
	mychar = fgetc(my_inc_parser->f);

	char mychar = fgetc(f);

#ifdef DEBUG_INCLUDE
	printf("%s : %c\n", __func__, mychar);
#endif

	switch (mychar) {
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
	char mychar = fgetc(f);

	char mychar;
	struct inc_parser *my_inc_parser;

	my_inc_parser = (struct inc_parser *)(myfsm->priv);
	mychar = fgetc(my_inc_parser->f);

#ifdef DEBUG_INCLUDE
	printf("%s : %c\n", __func__, mychar);
#endif

	pat->pos++;
	if (mychar == pat->str[pat->pos]) {
		if (pat->len - 1 == pat->pos)
			FSM_NEXT(myfsm, include_detected);
		else 
			FSM_NEXT(myfsm, detect_pattern);
	}

	switch (mychar) {
		case EOF  : FSM_STOP(myfsm);
		case '\n' :
		case '\r' : FSM_NEXT(myfsm, in_normal_mode);
		default   : FSM_NEXT(myfsm, skip_util_eol);
	}
}


static void include_detected(struct fsm *myfsm)
{
	struct pattern *pat = (struct pattern *)myfsm->data;
	char mychar = fgetc(f);

	char mychar;
	struct inc_parser *my_inc_parser;

	my_inc_parser = (struct inc_parser *)(myfsm->priv);
	mychar = fgetc(my_inc_parser->f);

#ifdef DEBUG_INCLUDE
	printf("%s : %c\n", __func__, mychar);
#endif

	pat->pos = 0;
	switch (mychar) {
		case EOF  : FSM_STOP(myfsm);
		case '\n' :
		case '\r' : printf("%c", mychar);
			    FSM_NEXT(myfsm, in_normal_mode);
		default   : printf("%c", mychar);
			    FSM_NEXT(myfsm, include_detected);
	}
}


static void skip_util_eol(struct fsm *myfsm)
{
	char mychar = fgetc(f);
	char mychar;
	struct inc_parser *my_inc_parser;

	my_inc_parser = (struct inc_parser *)(myfsm->priv);
	mychar = fgetc(my_inc_parser->f);

#ifdef DEBUG_INCLUDE
	printf("%s : %c\n", __func__, mychar);
#endif

	switch (mychar) {
		case EOF  : FSM_STOP(myfsm);
		case '\n' :
		case '\r' : FSM_NEXT(myfsm, in_normal_mode);
		default   : FSM_NEXT(myfsm, skip_util_eol);
	}
}

#ifdef TEST_INCLUDE

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;

	f = fopen(argv[1], "r");

	if (NULL == f)
		return 1;

	inc_parse(f);

	fclose(f);

	return 0;
}

#endif /* TEST_INCLUDE */
