#include <stdlib.h>
#include "list.h"

void gr_list_free(struct gr_list *l)
{
	struct gr_list_node *curr;
	struct gr_list_node *next;

	curr = l->first;
	if (!curr)
		goto finish;

	next = curr->next;
	while (next) {
		free(curr->data);
		free(curr);
		curr = next;
		next = next->next;
	}

	free(curr->data);
	free(curr);

finish:
	free(l);
}

void gr_list_append(struct gr_list *list, void *data)
{
	struct gr_list_node *new_node = gr_list_node_new();
	new_node->data = data;

	if (list->first == NULL) {
		list->first = new_node;
		list->last = new_node;
	} else if (list->first == list->last) {
		list->first->next = new_node;
		new_node->prev = list->first;
		list->last = new_node;
	} else {
		list->last->next = new_node;
		new_node->prev = list->last;
		list->last = new_node;
	}

	list->length++;
}

void gr_list_remove(struct gr_list *list, struct gr_list_node *n)
{
	if (list->length == 1) {
		list->first = NULL;
		list->last = NULL;
	} else {
		if (n == list->first) {
			list->first = list->first->next;
			list->first->prev = NULL;
		} else if (n == list->last) {
			list->last = list->last->prev;
			list->last->next = NULL;
		} else {
			n->prev->next = n->next;
			n->next->prev = n->prev;
		}
	}
	free(n->data);
	free(n);
	list->length--;
}

struct gr_list_node * gr_list_get(struct gr_list *list, size_t idx)
{
	struct gr_list_node *ret = list->first;

	if (idx >= list->length)
		return NULL;

	while (idx --> 0)
		ret = ret->next;

	return ret;
}