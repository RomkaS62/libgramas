#ifndef PTR_ARRAY_H
#define PTR_ARRAY_H

#include <stdlib.h>
#include "gr_export.h"

GR_CDECL_BEGIN

struct GR_EXPORT ptr_array {
	void **arr;
	size_t capacity;
	size_t used;
};

/* ptr_arr - struct ptr_array *.
 * el - pointer to whatever it is that you store in the array.
 * idx - index of the element.
 * */
#define ptr_arr_foreach(ptr_arr, el, idx)		\
	for (size_t idx = 0;				\
		(idx < (ptr_arr)->used)			\
			? el = (ptr_arr)->arr[idx], 1	\
			: 0;				\
		idx++)

GR_EXPORT void ptr_arr_init(struct ptr_array *arr, size_t init_size);
GR_EXPORT void ptr_arr_add(struct ptr_array *arr, void *ptr);
GR_EXPORT void ptr_arr_delete(struct ptr_array *arr);
GR_EXPORT int ptr_arr_set(struct ptr_array *arr, size_t idx, void *ptr);

GR_CDECL_END

#endif /* PTR_ARRAY_H */
