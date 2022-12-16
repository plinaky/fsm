#ifndef _GRAPH_H
#define _GRAPH_H

#include "list.h"


/*****************************************************************************/
/*                                 NODES                                     */
/*****************************************************************************/
struct node {
	struct list_head list;
	unsigned int id;
};

#define NEW_NODE(_NAME_) struct node _NAME_ = { \
	LIST_HEAD_INIT(_NAME_.list), 0          \
}

void print_node(struct node *my_node);

void add_node(struct node *my_node, unsigned int id);

#define ADD_NODE(_NODE_, _ID_) add_node( \
		(struct node *)(_NODE_), \
		(unsigned int)(_ID_))



/*****************************************************************************/
/*                       EDGES : or adjacence matrixes                       */
/*****************************************************************************/
/*
** An adjancence matrix is assumed to have a low density,
** thus it is represented by the chained list of its non null coeffs
*/

struct edge {
	struct list_head list;
	unsigned int i, j, v;
};

#define NEW_EDGE(_NAME_) struct edge _NAME_ = { \
	LIST_HEAD_INIT(_NAME_.list), 0, 0, 0 \
}

void print_edge(struct edge *my_edge);

void set_edge(struct edge *my_edge,
		unsigned int s1,
		unsigned int s2,
		unsigned int val);

void add_edge(struct edge *my_edge,
		unsigned int s1,
		unsigned int s2,
		unsigned int val);

#define INS_EDGE(_EDGE_, _S1_, _S2_, _V_) add_edge( \
		(struct edge *)(_EDGE_),            \
		(unsigned int)(_S1_),               \
		(unsigned int)(_S2_),               \
		(unsigned int)(_V_))



/*****************************************************************************/
/*              GRAPHS : simply a list of edges and a list of nodes          */
/*****************************************************************************/
struct graph {
	struct node nodes;
	struct edge edges;
};

#define NEW_GRAPH(_NAME_) struct graph _NAME_ = {     \
	{LIST_HEAD_INIT(_NAME_.nodes.list), 0},       \
	{LIST_HEAD_INIT(_NAME_.edges.list), 0, 0, 0}, \
}

#define ADD_EDGE(_GRAPH_, _S1_, _S2_, _V_) do {   \
	ADD_NODE(_GRAPH_.nodes, _S1_);            \
	ADD_NODE(_GRAPH_.nodes, _S2_);            \
	INS_EDGE(_GRAPH_.edges, _S1_, _S2_, _V_); \
} while(0)

#define PRINT_GRAPH(_GRAPH_) do {     \
	print_node(&(_GRAPH_.nodes)); \
	print_edge(&(_GRAPH_.edges)); \
} while(0)

#endif
