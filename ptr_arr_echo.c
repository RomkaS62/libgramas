#include <stdio.h>
#include "gramas/ptr_array.h"

int main(int argc, char **argv)
{
	struct ptr_array arr;
	char *str;
	int i;

	ptr_arr_init(&arr, 8);
	for (i = 1; i < argc; i++) {
		ptr_arr_add(&arr, argv[i]);
	}

	ptr_arr_foreach(&arr, str, tmp) {
		puts(str);
	}

	free(arr.arr);

	return 0;
}
