#ifndef PTR_ARRAY_H
#define PTR_ARRAY_H

#include <stdlib.h>

struct ptr_array {
	void **arr;
	size_t capacity;
	size_t used;
};

#define ptr_arr_foreach(ptr_arr, el, tmp)		\
	for (size_t tmp = 0;				\
		(tmp < (ptr_arr)->used)		\
			? el = (ptr_arr)->arr[tmp], 1	\
			: 0;				\
		tmp++)

void ptr_arr_init(struct ptr_array *arr, size_t init_size);
void ptr_arr_add(struct ptr_array *arr, void *ptr);
void ptr_arr_delete(struct ptr_array *arr);
int ptr_arr_set(struct ptr_array *arr, size_t idx, void *ptr);

#endif /* PTR_ARRAY_H */
