#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "preproc.h"
#include "fsm.h"
#include "scan.h"

#define TU_MAX_LEN 2000000u

//#define DEBUG_PREPROC
#define TEST_PREPROC


struct str_list {
	struct list_head list;
	char *str;
}

struct parser {
	char *buf;
	struct graph defines;
	unsigned int pos;
};


FSM_STATE(normal_mode, myfsm);
FSM_STATE(skip_until_eol, myfsm);
FSM_STATE(sharp_found, myfsm);

FSM_STATE(normal, myfsm);
FSM_STATE(insert, myfsm);
FSM_STATE(skip, myfsm);

FSM_STATE(insert, myfsm);
/* https://gcc.gnu.org/onlinedocs/cpp/Index-of-Directives.html */
static char * directive [] = {
	"define",
	"elif",
	"else",
	"endif",
	"if",
	"ifdef",
	"ifndef",
	"undef"
}

struct pattern {
	char str [PAT_MAX_SIZE];
	unsigned char len;
	unsigned char pos;
};

