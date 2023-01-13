#ifndef _FSM_
#define _FSM_


struct fsm {
	void (*state)(struct fsm *myfsm);
	void *priv;
};


#define FSM_DECLARE(_FSM_NAME_) \
	struct fsm _FSM_NAME_ = {NULL}


#define FSM_NEXT(_FSM_PTR_, _FSM_STATE_) do { \
	(_FSM_PTR_)->state = _FSM_STATE_; \
	return; \
} while(0)


#define FSM_STOP(_FSM_PTR_) \
	FSM_NEXT(_FSM_PTR_, NULL)


#define FSM_RUN(_FSM_PTR_, _FSM_STATE_) do { \
	(_FSM_PTR_)->state = _FSM_STATE_; \
	while ((_FSM_PTR_)->state != NULL) { \
		(_FSM_PTR_)->state(_FSM_PTR_); \
	} \
} while(0)


#endif
