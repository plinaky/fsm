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


FSM_STATE(normal_mode, myfsm);
FSM_STATE(word_mode,   myfsm);
FSM_STATE(char_found,  myfsm);
FSM_STATE(end_scan,    myfsm);

#ifdef DEBUG_SCAN

#define LOG_STATE(_PARSER_) do {             \
	printf("\nEntering %s (line %d)\n",  \
			__func__,            \
			__LINE__);           \
	print_parser(_PARSER_);              \
} while(0)

#else

#define LOG_STATE

#endif

static inline void print_parser(struct parser *my_parser)
{
	printf("res = %d, str = \"", my_parser->res);
	for (unsigned char i = 0 ; i < my_parser->pos ; i++)
		printf("%c", my_parser->str[i]);
	printf("\"\n");

}

FSM_STATE(normal_mode, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (mychar) {
		case EOF  : my_parser->res |= SCAN_EOF;
			    FSM_STOP(myfsm);
		case ' '  :
		case '\t' : FSM_NEXT(myfsm, normal_mode);
		case '\n' :
		case '\r' : FSM_STOP(myfsm);
		default   : my_parser->str[my_parser->pos++] = mychar;
			    FSM_NEXT(myfsm, char_found);
	}
}

FSM_STATE(word_mode, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	char mychar = fgetc(my_parser->f);

	LOG_STATE(my_parser);

	switch (mychar) {
		case EOF  : my_parser->res |= SCAN_EOF;
			    FSM_NEXT(myfsm, end_scan);
		case ' '  :
		case '\t' : FSM_NEXT(myfsm, end_scan);
		case '\n' :
		case '\r' : my_parser->res |= SCAN_EOL;
			    FSM_NEXT(myfsm, end_scan);
		default   : my_parser->str[my_parser->pos++] = mychar;
			    FSM_NEXT(myfsm, char_found);
	}
}

FSM_STATE(char_found, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	LOG_STATE(my_parser);

	my_parser->res |= SCAN_FOUND;

	if ((my_parser->max_len - 2) < my_parser->pos) {
		my_parser->res |= SCAN_BUF_FULL;
		FSM_NEXT(myfsm, end_scan);
	}

	FSM_NEXT(myfsm, word_mode);
}

FSM_STATE(end_scan, myfsm)
{
	FSM_DATA(myfsm, struct parser, my_parser);
	LOG_STATE(my_parser);
	my_parser->str[my_parser->pos++] = '\0';
	FSM_STOP(myfsm);
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

int main(int argc, char *argv[])
{
	FILE *f;
	int ret;
	char word_found[50];

	if (argc < 2)
		return 1;

	f = fopen(argv[1], "r");
	do {
		ret = STR_SCAN(f, word_found, 50);

		if (ret & SCAN_FOUND)
			printf("%s ", word_found);

		if (ret & SCAN_EOL)
			printf("\n");

	} while (!(ret & SCAN_EOF));

	printf("\n\n***************************************************************************\n");
	return 0;
}

#endif
