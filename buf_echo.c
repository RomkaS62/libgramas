#include <stdio.h>
#include "buf.h"

int main(int argc, char **argv)
{
	char *str;
	struct buffer buf;
	int i;

	buf_init(&buf, 8, sizeof(char *));
	for (i = 1; i < argc; i++) {
		buf_append(&buf, argv[i]);
	}

	buf_foreach(&buf, str, tmp) {
		puts(str);
	}

	buf_delete(&buf);

	return 0;
}
