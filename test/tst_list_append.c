#include <stdlib.h>
#include <stdio.h>
#include "list.h"

static int data[] = {1, 2, 3, 4, 5, 6};
static int *data_ptr[sizeof(data) / sizeof(data[0])];

static void init_data()
{
	for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
		data_ptr[i] = malloc(sizeof(data_ptr[0]));
		*(data_ptr[i]) = data[i];
	}
}

int main(void)
{
	struct gr_list *list = gr_list_new();
	struct gr_list_node *node;
	int *num;
	size_t i;

	init_data();

	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
		gr_list_append(list, data_ptr[i]);
	}

	gr_list_foreach(list, node, num, i) {
		if (data[i] != *num)
			return 1;
	}

	gr_list_free(list);

	return 0;
}