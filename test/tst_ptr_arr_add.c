#include <stdio.h>
#include "ptr_array.h"

int main(void)
{
	int i;
	int *ptr;
	int ret = 0;
	struct ptr_array arr;

	ptr_arr_init(&arr, 8);
	for (i = 0; i < 16; i++) {
		ptr = malloc(sizeof(*ptr));
		*ptr = i;
		ptr_arr_add(&arr, ptr);
	}
	ptr_arr_foreach(&arr, ptr, j) {
		if (*ptr != j) {
			goto fail;
		}
	}
	goto end;

fail:
	ret = 1;
end:
	ptr_arr_foreach(&arr, ptr, tmp) {
		free(ptr);
	}
	ptr_arr_delete(&arr);
	return ret;
}
