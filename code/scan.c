#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"
#include "scan.h"

//#define DEBUG_SCAN
#define TEST_SCAN

struct parser {
	FILE *f;
	char *str;
	unsigned char pos;
	int res;
	unsigned int max_len;
};


static void normal_mode(struct fsm *myfsm);
static void word_mode(struct fsm *myfsm);
static void char_found(struct fsm *myfsm);

static void normal_mode(struct fsm *myfsm)
{
	char mychar;
	struct parser *my_parser;

	my_parser = (struct parser*)(myfsm->priv);
	mychar = fgetc(my_parser->f);

#ifdef DEBUG_SCAN
	printf("%d : %s %d\n", my_parser->res, __func__, __LINE__);
#endif
	switch (mychar) {
		case EOF  :
			my_parser->res |= SCAN_EOF;
			FSM_STOP(myfsm);
		case ' '  :
		case '\t' :
			FSM_NEXT(myfsm, normal_mode);
		case '\n' :
		case '\r' :
			FSM_STOP(myfsm);
		default   :
			my_parser->str[my_parser->pos++] = mychar;
			FSM_NEXT(myfsm, char_found);
	}
}

static void word_mode(struct fsm *myfsm)
{
	char mychar;
	struct parser *my_parser;

	my_parser = (struct parser*)(myfsm->priv);
	mychar = fgetc(my_parser->f);

#ifdef DEBUG_SCAN
	printf("%d : %s %d\n", my_parser->res, __func__, __LINE__);
#endif
	switch (mychar) {
		case EOF  :
			my_parser->res |= SCAN_EOF;
			my_parser->str[my_parser->pos++] = '\0';
			FSM_STOP(myfsm);
		case ' '  :
		case '\t' :
			my_parser->str[my_parser->pos++] = '\0';
			FSM_STOP(myfsm);
		case '\n' :
		case '\r' :
			my_parser->res |= SCAN_EOL;
			my_parser->str[my_parser->pos++] = '\0';
			FSM_STOP(myfsm);
		default   :
			my_parser->str[my_parser->pos++] = mychar;
			FSM_NEXT(myfsm, char_found);
	}
}

static void char_found(struct fsm *myfsm)
{
	char mychar;
	struct parser *my_parser;

	my_parser = (struct parser*)(myfsm->priv);
	my_parser->res |= SCAN_FOUND;

#ifdef DEBUG_SCAN
	printf("%d : %s %d\n", my_parser->res, __func__, __LINE__);
#endif
	if ((my_parser->max_len - 2) < my_parser->pos) {
		my_parser->res |= SCAN_BUF_FULL;
		my_parser->str[my_parser->pos++] = '\0';
		FSM_STOP(myfsm);
	} else {
		FSM_NEXT(myfsm, word_mode);
	}
}

unsigned char str_scan(FILE *f, char *str, unsigned int max_len)
{
	if (NULL == f)
		return SCAN_NO_FILE;

	max_len++;

	if (max_len < 1)
		return SCAN_BUF_FULL;

	if (max_len > MAX_STR_LEN - 1)
		max_len = MAX_STR_LEN - 1;

	struct parser my_parser = {
		f,
		str,
		0,
		SCAN_NOP,
		max_len
	};

	str[0] = '\0';

	FSM_DECLARE(parser_fsm);
	parser_fsm.priv = (void *)(&my_parser);
	FSM_RUN(&parser_fsm, normal_mode);

	return my_parser.res;
}

#ifdef TEST_SCAN

int main (int argc, char *argv[])
{
	FILE *f;
	int ret;
	char word_found[150];

	if (argc < 2)
		return 1;

	f = fopen(argv[1], "r");
	do {
		ret = STR_SCAN(f, word_found, 150);
#ifdef DEBUG_SCAN
		printf("%+d : ", ret);
#endif
		if (ret & SCAN_FOUND)
			printf("%s ", word_found);

		if (ret & SCAN_EOL)
			printf("\n");

	} while (!(ret & SCAN_EOF));

	printf("\n\n***************************************************************************\n");
	return 0;
}

#endif
