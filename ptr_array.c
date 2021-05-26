#include <stdlib.h>
#include <stdio.h>
#include "ptr_array.h"

#define TRIM_COEFF	(2)
#define ENLARGE_COEFF	(2)

static void ensure_capacity(struct ptr_array *arr, size_t required_size);
static void trim(struct ptr_array *arr);
static int within_bounds(struct ptr_array *arr, size_t idx);

void ptr_arr_init(struct ptr_array *arr, size_t init_size)
{
	size_t i;

	if (arr == NULL) return;
	if (init_size == 0) {
		init_size = 1;
	}
	arr->arr = malloc(sizeof(arr->arr[0]) * init_size);
	for (i = 0; i < init_size; i++) {
		arr->arr[i] = NULL;
	}
	arr->capacity = init_size;
	arr->used = 0;
}

void ptr_arr_add(struct ptr_array *arr, void *ptr)
{
	ensure_capacity(arr, arr->used + 1);
	arr->arr[arr->used++] = ptr;
}

void ptr_arr_delete(struct ptr_array *arr)
{
	free(arr->arr);
	arr->capacity = 0;
	arr->used = 0;
	arr->arr = NULL;
}

int ptr_arr_set(struct ptr_array *arr, size_t idx, void *ptr)
{
	int ret = 0;
	if (within_bounds(arr, idx)) {
		arr->arr[idx] = ptr;
	} else {
		ret = 1;
	}
	return ret;
}

static void ensure_capacity(struct ptr_array *arr, size_t required_size)
{
	size_t new_size = required_size;

	if (required_size > arr->capacity) {
		if (required_size / ENLARGE_COEFF > arr->capacity) {
			new_size = required_size;
		} else {
			new_size = arr->capacity * ENLARGE_COEFF;
		}
		arr->capacity = new_size;
		arr->arr = realloc(arr->arr, new_size * sizeof(arr->arr[0]));
	}
}

static void trim(struct ptr_array *arr)
{
	if (arr->capacity / arr->used >= TRIM_COEFF) {
		arr->arr = realloc(arr->arr, arr->used * sizeof(arr->arr[0]));
	}
}

static int within_bounds(struct ptr_array *arr, size_t idx)
{
	return arr->used > idx;
}
