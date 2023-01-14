#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "preproc.h"
#include "fsm.h"

/* https://gcc.gnu.org/onlinedocs/cpp/Index-of-Directives.html */
static char * directive [] = {
	"assert",
	"define",
	"elif",
	"else",
	"endif",
	"error",
	"ident",
	"if",
	"ifdef",
	"ifndef",
	"import",
	"include",
	"include_next",
	"line",
	"pragma endregion",
	"pragma GCC dependency",
	"pragma GCC error",
	"pragma GCC poison",
	"pragma GCC system_header",
	"pragma GCC system_header",
	"pragma GCC warning",
	"pragma once",
	"pragma region",
	"sccs",
	"unassert",
	"undef",
	"warning"
}

struct pattern {
	char str [PAT_MAX_SIZE];
	unsigned char len;
	unsigned char pos;
};

char directive[PAT_MAX_SIZE];

struct parser {
	FILE *f;	
};

static void normal_mode(struct fsm *myfsm);
static void preproc_mode(struct fsm *myfsm);

static void in_word(struct fsm *myfsm);
static void in_space(struct fsm *myfsm);

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
	FSM_RUN(&parser, normal_mode);
}


static void normal_mode(struct fsm *myfsm)
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
		case '\r' : FSM_NEXT(myfsm, normal_mode);
		case '#'  : FSM_NEXT(myfsm, hash_detected);
		default   : FSM_NEXT(myfsm, skip_util_eol);
	}
}


static void hash_detected(struct fsm *myfsm)
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
		case '\t' : FSM_NEXT(myfsm, hash_detected);
		case '\n' :
		case '\r' : FSM_NEXT(myfsm, normal_mode);
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
		case '\r' : FSM_NEXT(myfsm, normal_mode);
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
			    FSM_NEXT(myfsm, normal_mode);
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
		case '\r' : FSM_NEXT(myfsm, normal_mode);
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
