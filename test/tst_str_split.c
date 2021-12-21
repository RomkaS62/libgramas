#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gramas/str.h"

const char str[] = "123.456.789";
const char part1[] = "123";
const char part2[] = "456";
const char part3[] = "789";

static void fail_if_not_equal(const char *str1, const char *str2)
{
	size_t len1;
	size_t len2;

	len1 = strlen(str1);
	len2 = strlen(str2);
	if (len1 != len2) {
		fprintf(stderr, "FAIL: string lengths not equal: %zu vs %zu\n"
				"%s\n"
				"%s\n",
				len1, len2,
				str1, str2);
		exit(1);
	}

	if (strcmp(str1, str1) != 0) {
		fprintf(stderr, "Strings not equal: \"%s\" vs \"%s\"\n",
				str1, str2);
		exit(1);
	}
}

int main(void)
{
	struct str_array parts;

	parts = gr_str_split(str, '.');

	if (parts.length != 3) {
		fprintf(stderr, "Did not get the epxected number of parts (3). Instead got %zu.\n",
				parts.length);
		return 1;
	}

	fail_if_not_equal(parts.strings[0], part1);
	fail_if_not_equal(parts.strings[1], part2);
	fail_if_not_equal(parts.strings[2], part2);

	gr_str_array_delete(&parts);

	return 0;
}
