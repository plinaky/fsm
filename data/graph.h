#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>


#define OFFSET(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)


#define CONTAINER(PTR, TYPE, MEMBER) \
	((TYPE *)((void *)(PTR) - OFFSET(TYPE, MEMBER)))


#define FOR_EACH_BRO(POS, HEAD, MEMBER)				\
	for (POS = CONTAINER(HEAD, typeof(*POS), MEMBER);	\
	     &POS->MEMBER != (HEAD);			        \
	     POS = CONTAINER(POS->MEMBER.bro, typeof(*POS), MEMBER))


#define FOR_EACH_SON(POS, HEAD, MEMBER)				\
	for (POS = CONTAINER(HEAD, typeof(*POS), MEMBER);	\
	     &POS->MEMBER != (HEAD);			        \
	     POS = CONTAINER(POS->MEMBER.son, typeof(*POS), MEMBER))


#define NODE(_NAME_) struct node _NAME_ = {{&_NAME_, &_NAME_}}


struct node {
	struct node *p[2];
};


static inline void ins_node(struct node *new, struct node *pos, unsigned char level)
{
	struct node *tmp;

	if (new == pos)
		return;

	tmp = pos->p[level];
	pos->p[level] = new;
	new->p[level] = tmp;
}



static inline struct node *get_prev(struct node *pos, unsigned char level)
{
	struct node *cur = pos;

	while (cur->p[level] != pos)
		cur = cur->p[level];

	return cur;
}

static inline struct node *rem_bro(struct node *top, struct node *pos)
{
	struct node *cur;

	if (NULL == cur || NULL == pos)
		return;

	if (pos == top) {
		cur = top->bro;
		top->bro = NULL;
		return cur;
	}

	cur = top;

	while (NULL != cur->bro) {
		if (pos == cur->bro) {
			cur->bro = cur->bro->bro;
			pos->bro = NULL;
			break;
		}
	}

	return top;
}

static inline struct node *rem_son(struct node *top, struct node *pos)
{
	struct node *cur;

	if (NULL == cur || NULL == pos)
		return NULL;

	if (pos == top) {
		cur = top->son;
		top->son = NULL;
		return cur;
	}

	cur = top;

	while (NULL != cur->son) {
		if (pos == cur->son) {
			cur->son = cur->son->son;
			pos->son = NULL;
			break;
		}
	}

	return top;
}

static inline struct node *rem_node(struct node *top, struct node *pos)
{
	struct node *cur = top;

	while (NULL != cur) {
		rem_bro()
	} 

	return top;
}
