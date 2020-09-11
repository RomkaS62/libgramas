#include <stdio.h>
#include "ptr_array.h"

int main(void)
{
	int i;
	int *ptr;
	struct ptr_array arr;

	ptr_arr_init(&arr, 8);
	for (i = 0; i < 16; i++) {
		ptr = malloc(sizeof(*ptr));
		*ptr = i;
		ptr_arr_add(&arr, ptr);
	}
	ptr_arr_foreach(&arr, ptr, tmp) {
		printf("%i\n", *ptr);
	}
	ptr_arr_foreach(&arr, ptr, tmp) {
		free(ptr);
	}
	ptr_arr_delete(&arr);
	return 0;
}
