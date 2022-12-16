#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "fsm.h"

#define SIZE 10u
#define NOD(_A_, _B_) !((_A_) % (_B_))

static int data[SIZE];
static unsigned int r = 7291;

static void a00(struct fsm *myfsm);
static void a01(struct fsm *myfsm);
static void a02(struct fsm *myfsm);
static void a03(struct fsm *myfsm);
static void a04(struct fsm *myfsm);
static void a05(struct fsm *myfsm);
static void a06(struct fsm *myfsm);
static void a07(struct fsm *myfsm);
static void a08(struct fsm *myfsm);
static void a09(struct fsm *myfsm);
static void a10(struct fsm *myfsm);
static void a11(struct fsm *myfsm);
static void a12(struct fsm *myfsm);
static void a13(struct fsm *myfsm);
static void a14(struct fsm *myfsm);
static void a15(struct fsm *myfsm);
static void a16(struct fsm *myfsm);
static void a17(struct fsm *myfsm);
static void a18(struct fsm *myfsm);
static void a19(struct fsm *myfsm);

static void trace(void)
{
	for (int i = 0 ; i < SIZE ; i++)
		printf("0x%08X\t", data[i]);
	printf("\n");
}

static inline void TOSS(void)
{
	r = (unsigned int)rand();
}

static inline void STORE(void)
{
	data[r % SIZE] = r;
}

static inline void route(struct fsm *myfsm)
{
	if (NOD(r, 120011))
		FSM_STOP(myfsm);
	else switch (r % 20) {
	case 0  : FSM_NEXT(myfsm, a00);
	case 1  : FSM_NEXT(myfsm, a01);
	case 2  : FSM_NEXT(myfsm, a02);
	case 3  : FSM_NEXT(myfsm, a03);
	case 4  : FSM_NEXT(myfsm, a04);
	case 5  : FSM_NEXT(myfsm, a05);
	case 6  : FSM_NEXT(myfsm, a06);
	case 7  : FSM_NEXT(myfsm, a07);
	case 8  : FSM_NEXT(myfsm, a08);
	case 9  : FSM_NEXT(myfsm, a09);
	case 10 : FSM_NEXT(myfsm, a10);
	case 11 : FSM_NEXT(myfsm, a11);
	case 12 : FSM_NEXT(myfsm, a12);
	case 13 : FSM_NEXT(myfsm, a13);
	case 14 : FSM_NEXT(myfsm, a14);
	case 15 : FSM_NEXT(myfsm, a15);
	case 16 : FSM_NEXT(myfsm, a16);
	case 17 : FSM_NEXT(myfsm, a17);
	case 18 : FSM_NEXT(myfsm, a18);
	case 19 :
	default : FSM_NEXT(myfsm, a19);
	}
}

static void a00(struct fsm *myfsm) {TOSS(); r += 11; STORE(); route(myfsm);}
static void a01(struct fsm *myfsm) {TOSS(); r -= 13; STORE(); route(myfsm);}
static void a02(struct fsm *myfsm) {TOSS(); r *= 15; STORE(); route(myfsm);}
static void a03(struct fsm *myfsm) {TOSS(); r /= 17; STORE(); route(myfsm);}
static void a04(struct fsm *myfsm) {TOSS(); r += 19; STORE(); route(myfsm);}
static void a05(struct fsm *myfsm) {TOSS(); r -= 21; STORE(); route(myfsm);}
static void a06(struct fsm *myfsm) {TOSS(); r *= 23; STORE(); route(myfsm);}
static void a07(struct fsm *myfsm) {TOSS(); r /= 25; STORE(); route(myfsm);}
static void a08(struct fsm *myfsm) {TOSS(); r += 27; STORE(); route(myfsm);}
static void a09(struct fsm *myfsm) {TOSS(); r -= 29; STORE(); route(myfsm);}
static void a10(struct fsm *myfsm) {TOSS(); r *= 31; STORE(); route(myfsm);}
static void a11(struct fsm *myfsm) {TOSS(); r /= 33; STORE(); route(myfsm);}
static void a12(struct fsm *myfsm) {TOSS(); r += 35; STORE(); route(myfsm);}
static void a13(struct fsm *myfsm) {TOSS(); r -= 37; STORE(); route(myfsm);}
static void a14(struct fsm *myfsm) {TOSS(); r *= 39; STORE(); route(myfsm);}
static void a15(struct fsm *myfsm) {TOSS(); r /= 41; STORE(); route(myfsm);}
static void a16(struct fsm *myfsm) {TOSS(); r += 43; STORE(); route(myfsm);}
static void a17(struct fsm *myfsm) {TOSS(); r -= 45; STORE(); route(myfsm);}
static void a18(struct fsm *myfsm) {TOSS(); r *= 47; STORE(); route(myfsm);}
static void a19(struct fsm *myfsm) {TOSS(); r /= 49; STORE(); route(myfsm);}

int main(void)
{
	FSM_DECLARE(tosser);
	trace();
	srand(time(NULL));
	FSM_RUN(&tosser, a00);
	trace();
	return 0;
}

