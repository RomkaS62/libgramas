#include <string.h>
#include <stdio.h>
#include "str.h"

char data[] = {1, 2, 3, 4};
char expected[] = "01 02 03 04";

int main(void)
{
	char *hexbuf = NULL;

	gr_buf2hex(&hexbuf, data, sizeof(data));
	puts(expected);
	puts(hexbuf);

	return strncmp(expected, hexbuf, sizeof(expected) - 1);
}
