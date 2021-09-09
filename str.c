#include <stdlib.h>
#include <stdio.h>
#include "str.h"

void gr_buf2hex(char **str, const char *data, size_t len)
{
	char *ptr;

	if (*str == NULL)
		*str = realloc(*str, len * 3 + 1);

	ptr = *str;
	while (len --> 0) {
		*ptr++ = gr_nib2hex(gr_hinib(*data));
		*ptr++ = gr_nib2hex(gr_lonib(*data));
		*ptr++ = ' ';
		data++;
	}
	*ptr = '\0';
}
