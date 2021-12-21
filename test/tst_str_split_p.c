#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gramas/str.h"

const char str[] = "123.456";
const char part1[] = "123";
const char part2[] = "456";

static void fail_if_not_equal(const char *str1, const char *str2)
{
	size_t len1;
	size_t len2;

	len1 = strlen(str1);
	len2 = strlen(str2);
	if (len1 != len2) {
		fprintf(stderr, "FAIL: string lengths not equal: %zu vs %zu",
				len1, len2);
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
	char split_part1[sizeof(part1)];
	char split_part2[sizeof(part2)];

	gr_str_split_p(str, '.', 2, split_part1, split_part2);

	fail_if_not_equal(split_part1, part1);
	fail_if_not_equal(split_part2, part2);

	return 0;
}
