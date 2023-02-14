#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"

#define TEST_INCLUDE
//#define DEBUG_INCLUDE


#define PAT_MAX_SIZE 32u

struct parser {
	FILE *f;	
	char directive[PAT_MAX_SIZE];
};

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

FSM_STATE(in_normal_mode,         myfsm);
FSM_STATE(entering_hash_detected, myfsm);
FSM_STATE(detect_pattern,         myfsm);
FSM_STATE(include_detected,       myfsm);
FSM_STATE(skip_util_eol,          myfsm);


#ifdef DEBUG_INCLUDE

#define LOG_STATE(_PARSER_) do {                                 \
	printf("\nEntering %s (line %d)\n", __func__, __LINE__); \
} while(0)

#else

#define LOG_STATE

#endif


void inc_parse(FILE *f)
{
	struct parser my_parser = {
		f,
		"\0"
	};

	if (NULL == f)
		return 1;
	
	FSM_DECLARE(parser);
	parser.priv = (void *)(& my_parser);
	FSM_RUN(&parser, in_normal_mode);
}


FSM_STATE(in_normal_mode, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

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


FSM_STATE(entering_hash_detected, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);


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


FSM_STATE(detect_pattern, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);


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


FSM_STATE(include_detected, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (mychar) {
		case EOF  : FSM_STOP(myfsm);
		case '\n' :
		case '\r' : printf("%c", mychar);
			    FSM_NEXT(myfsm, in_normal_mode);
		default   : printf("%c", mychar);
			    FSM_NEXT(myfsm, include_detected);
	}
}


FSM_STATE(skip_util_eol, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (mychar) {
		case EOF  : FSM_STOP(myfsm);
		case '\n' :
		case '\r' : FSM_NEXT(myfsm, in_normal_mode);
		default   : FSM_NEXT(myfsm, skip_util_eol);
	}
}


void inc_parse(FILE *f)
{
	struct parser my_parser = {
		f,
		"\0"
	};

	if (NULL == f)
		return 1;
	
	FSM_DECLARE(parser);
	parser.priv = (void *)(& my_parser);
	FSM_RUN(&parser, in_normal_mode);
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
