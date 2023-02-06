#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"
#include "list.h"
#include "scan.h"

//#define DEBUG_SCAN
#define TEST_SCAN

struct parser {
	FILE *f;
	char *str;
	unsigned char pos;
	int res;
	unsigned int max_len;
	FSM_DECLARE(operator);
};


FSM_STATE(normal_mode, myfsm);
FSM_STATE(word_mode,   myfsm);
FSM_STATE(char_found,  myfsm);
FSM_STATE(end_scan,    myfsm);

#ifdef DEBUG_SCAN

#define LOG_STATE(_PARSER_) do {                                   \
	printf("\nEntering %s (line %d)\n",  __func__, 	__LINE__); \
	print_parser(_PARSER_);                                    \
} while(0)

#else

#define LOG_STATE

#endif

static inline void print_parser(struct parser *myparser)
{
	printf("res = %d, str = \"", myparser->res);
	for (unsigned char i = 0 ; i < myparser->pos ; i++)
		printf("%c", myparser->str[i]);
	printf("\"\n");

}

FSM_STATE(normal_mode, myfsm)
{
	struct parser *myparser = container_of(myfsm, struct parser, operator);
	char mychar = fgetc(myparser->f);

	LOG_STATE(myparser);

	switch (mychar) {
		case EOF  : myparser->res |= SCAN_EOF;
			    FSM_STOP(myfsm);
		case ' '  :
		case '\t' : FSM_NEXT(myfsm, normal_mode);
		case '\n' :
		case '\r' : FSM_STOP(myfsm);
		default   : myparser->str[myparser->pos++] = mychar;
			    FSM_NEXT(myfsm, char_found);
	}
}

FSM_STATE(word_mode, myfsm)
{
	struct parser *myparser = container_of(myfsm, struct parser, operator);
	char mychar = fgetc(myparser->f);

	LOG_STATE(myparser);

	switch (mychar) {
		case EOF  : myparser->res |= SCAN_EOF;
			    FSM_NEXT(myfsm, end_scan);
		case ' '  :
		case '\t' : FSM_NEXT(myfsm, end_scan);
		case '\n' :
		case '\r' : myparser->res |= SCAN_EOL;
			    FSM_NEXT(myfsm, end_scan);
		default   : myparser->str[myparser->pos++] = mychar;
			    FSM_NEXT(myfsm, char_found);
	}
}

FSM_STATE(char_found, myfsm)
{
	struct parser *myparser = container_of(myfsm, struct parser, operator);

	LOG_STATE(myparser);
	myparser->res |= SCAN_FOUND;

	if ((myparser->max_len - 2) < myparser->pos) {
		myparser->res |= SCAN_BUF_FULL;
		FSM_NEXT(myfsm, end_scan);
	}

	FSM_NEXT(myfsm, word_mode);
}

FSM_STATE(end_scan, myfsm)
{
	struct parser *myparser = container_of(myfsm, struct parser, operator);

	LOG_STATE(myparser);
	myparser->str[myparser->pos++] = '\0';
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

	struct parser myparser = {
		f,
		str,
		0,
		SCAN_NOP,
		max_len
	};

	str[0] = '\0';

	FSM_RUN(&myparser.operator, normal_mode);

	return myparser.res;
}

#ifdef TEST_SCAN

int main(int argc, char *argv[])
{
	FILE *f;
	int ret;
	char word_found[MAX_STR_LEN];

	if (argc < 2)
		return 1;

	f = fopen(argv[1], "r");
	do {
		ret = str_scan(f, word_found, MAX_STR_LEN - 1);

		if (ret & SCAN_FOUND)
			printf("%s ", word_found);

		if (ret & SCAN_EOL)
			printf("\n");

	} while (!(ret & SCAN_EOF));

	return 0;
}

#endif
