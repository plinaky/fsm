#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fsm.h"
#include "graph.h"

#define SIZE 10u
#define NOD(_A_, _B_) !((_A_) % (_B_))

static int data[SIZE];

static void a0(struct fsm *myfsm);
static void a1(struct fsm *myfsm);

static NEW_GRAPH(graph_str);

static void trace(void)
{
	for (int i = 0 ; i < SIZE ; i++)
		printf("0x%08X\t", data[i]);
	printf("\n");
}

static inline unsigned int toss(void)
{
	unsigned int r = (unsigned int)rand();
	data[r % SIZE] = r;
	return r;
}

static void a0(struct fsm *myfsm)
{
	unsigned int r = toss();

	if (NOD(r, 700000000))
		FSM_STOP(myfsm);
	else switch (r % 47) {
	case 0  :
		ADD_EDGE(&graph_str, 0, 0, 1);
		FSM_NEXT(myfsm, a0);
	default :
		ADD_EDGE(&graph_str, 0, 1, 1);
		FSM_NEXT(myfsm, a1);
	}
}

static void a1(struct fsm *myfsm)
{
	unsigned int r = toss();

	if (NOD(r, 30000000)) {
		PRINT_GRAPH(graph_str);
		trace();
	}
	switch (r % 97) {
	case 0  :
		ADD_EDGE(&graph_str, 1, 0, 1);
		FSM_NEXT(myfsm, a0);
	default :
		ADD_EDGE(&graph_str, 1, 1, 1);
		FSM_NEXT(myfsm, a1);
	}
}



int main(void)
{
	FSM_DECLARE(tosser);
	trace();
	srand(time(NULL));
	FSM_RUN(&tosser, a0);
	return 0;
}

