#include <stdio.h>
#include <string.h>
#include "gramas/str.h"

static size_t group = 2;
static size_t groups = 2;
static const char data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
static char expected_str[] =
	"0102 0304\n"
	"0506 0708\n"
	"090A 0B\n"
	;
static char hexbuf[sizeof(expected_str)];

int main(void)
{
	size_t buflen;

	if (buflen = gr_hexdump_len(sizeof(data), group, groups) != sizeof(expected_str)) {
		printf("Expected length: %zu\nActual length: %zu\n",
				sizeof(expected_str), buflen);
		return 1;
	}
	gr_hexdump(data, sizeof(data), hexbuf, group, groups);
	puts(expected_str);
	puts(hexbuf);
	return memcmp(expected_str, hexbuf, sizeof(expected_str));
}
