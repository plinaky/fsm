#ifndef _FSM_
#define _FSM_

struct fsm {
	void (*state)(struct fsm *myfsm);
};


#define FSM_DECLARE(_FSM_) struct fsm _FSM_

#define FSM_STATE(_STATE_, _FSM_) static void _STATE_(struct fsm *_FSM_)

#define FSM_NEXT(_FSM_, _STATE_) do {  \
	(_FSM_)->state = _STATE_;      \
	return;                        \
} while(0)

#define FSM_STOP(_FSM_) FSM_NEXT(_FSM_, NULL)

static inline void FSM_RUN(struct fsm *myfsm, void (*start)(struct fsm *))
{
	myfsm->state = start;

	while (myfsm->state != NULL)
		myfsm->state(myfsm);
}

#endif
