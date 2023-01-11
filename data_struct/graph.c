#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

/*****************************************************************************/
/*                                 NODES                                     */
/*****************************************************************************/
void print_node(struct node *my_node)
{
	struct node *pos = NULL;

	printf("\n****************\n");
	list_for_each_entry(pos, &(my_node->list), list)
		printf("%u \n", pos->id);
}

void add_node(struct node *my_node, unsigned int id)
{
	struct node *pos = NULL;
	struct node *elt = NULL;

	list_for_each_entry(pos, &(my_node->list), list) {

		if (pos->id == id)
			return;

		if (pos->id > id)
			break;
	}

	/* if not, try to create one */
	elt = calloc(1, sizeof(struct node));

	if (NULL == elt)
		return;

	elt->id = id;
	list_add_tail(&(elt->list), &(pos->list));
}


/*****************************************************************************/
/*                       EDGES : or adjacence matrixes                       */
/*****************************************************************************/
void print_edge(struct edge *my_edge)
{
	struct edge *pos = NULL;

	printf("\n****************\n");
	list_for_each_entry(pos, &(my_edge->list), list) 
		printf("%u %u %u \n", pos->i, pos->j, pos->v);
}


void set_edge(struct edge *my_edge,
		unsigned int s1,
		unsigned int s2,
		unsigned int val)
{
	my_edge->i = s1;
	my_edge->j = s2;
	my_edge->v += val;
}


void add_edge(struct edge *my_edge,
		unsigned int s1,
		unsigned int s2,
		unsigned int val)
{
	struct edge *pos = NULL;
	struct edge *elt = NULL;

	list_for_each_entry(pos, &(my_edge->list), list) {
		/* look if edge exists */
		if ((pos->i == s1) && (pos->j == s2)) {
			set_edge(pos, s1, s2, val);
			return;
		}
		/* add edges in lexicographical order */
		if ((pos->i > s1) || ((pos->i == s1) && (pos->j > s2)))
			break;
	}

	/* if not, try to create one */
	elt = calloc(1, sizeof(struct edge));

	if (NULL == elt)
		return;

	set_edge(elt, s1, s2, val);
	list_add_tail(&(elt->list), &(pos->list));
}

