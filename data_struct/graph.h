#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include "list.h"

/*****************************************************************************/
/*                       LISTS                                               */
/*****************************************************************************/
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);					\
	((type *)(__mptr - offsetof(type, member))); })

struct node {
	struct node *next, *prev;
};

#define list_for_each_entry(pos, head, member)				\
	for (pos = container_of((head)->next, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = container_of(pos->member.next, typeof(*pos), member))

static inline void list_add_tail(struct node *new, struct node *head)
{
	head->prev = new;
	new->next = head;
	new->prev = prev;
	head->prev->next = new;

}

/*****************************************************************************/
/*                       EDGES : or adjacence matrixes                       */
/*****************************************************************************/
/*
** An adjancence matrix is assumed to have a low density,
** thus it is represented by the chained list of its non null coeffs
*/

struct edge {
	struct list_head list;
	void *s1, *s2;
};

#define NEW_EDGE(_NAME_) struct edge _NAME_ = { \
	{&(_NAME_.list), &(_NAME_.list)}, NULL, NULL, 0 \
}

static inline void print_edge(struct edge *my_edge)
{
	struct edge *pos = NULL;

	printf("\n****************\n");
	list_for_each_entry(pos, &(my_edge->list), list) 
		printf("%p %p \n", pos->i, pos->j);
}



static inline void add_edge(struct edge *my_edge, void *s1, void *s2)
{
	struct edge *pos = NULL;
	struct edge *elt = NULL;

	list_for_each_entry(pos, &(my_edge->list), list) {

		/* look if edge exists */
		if ((pos->s1 == s1) && (pos->s2 == s2))
			return;

		/* add edges in lexicographical order */
		if ((pos->i > s1) || ((pos->i == s1) && (pos->j > s2)))
			break;
	}

	/* if not, try to create one */
	elt = calloc(1, sizeof(struct edge));

	if (NULL == elt)
		return;

	elt->s1 = s1;
	elt->s2 = s2;
	list_add_tail(&(elt->list), &(pos->list));
}
