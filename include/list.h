#ifndef LIBGRAMAS_LIST_H
#define LIBGRAMAS_LIST_H

#include <stdlib.h>
#include "gr_export.h"

GR_CDECL_BEGIN

struct gr_list {
	struct gr_list_node *first;
	struct gr_list_node *last;
	size_t length;
};

struct gr_list_node {
	void *data;
	struct gr_list_node *next;
	struct gr_list_node *prev;
};

static inline struct gr_list * gr_list_new()
{
	return (struct gr_list *)calloc(1, sizeof(struct gr_list));
}

static inline struct gr_list_node * gr_list_node_new()
{
	return (struct gr_list_node *)calloc(1, sizeof(struct gr_list_node));
}

void gr_list_free(struct gr_list *l);
void gr_list_append(struct gr_list *list, void *data);
void gr_list_remove(struct gr_list *list, struct gr_list_node *n);
struct gr_list_node * gr_list_get(struct gr_list *list, size_t idx);

#define gr_list_foreach(_list, _node, _data, _idx)		\
	for (_idx = 0, _node = (_list)->first;			\
		_data = (_node) ? _node->data : NULL, _node;	\
		_idx++, _node = _node->next)

GR_CDECL_END

#endif /* LIBGRAMAS_LIST_H */