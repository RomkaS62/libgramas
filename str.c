#include <stdlib.h>
#include <stdio.h>
#include "gramas/str.h"

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

void gr_hexdump(const char *data,
		size_t len,
		char *str,
		size_t group,
		size_t groups)
{
	size_t i = 0;

	while (i < len) {
		if (i) {
			if (!(i % (group * groups))) {
				*str++ = '\n';
			} else if (!(i % group)) {
				*str++ = ' ';
			}
		}
		*str++ = gr_nib2hex(gr_hinib(*data));
		*str++ = gr_nib2hex(gr_lonib(*data++));
		i++;
	}
	if (len % (group * groups))
		*str++ = '\n';
	*str++ = '\0';
}

size_t gr_hexdump_len(size_t data_len, size_t group, size_t groups)
{
	size_t ret = 1;
	size_t full_lines;
	size_t line_chars;
	size_t rem_bytes;
	size_t rem_groups;
	size_t rem_grp_bytes;

	if (!group || !groups || !data_len)
		return 0;

	full_lines = data_len / (group * groups);
	rem_bytes = data_len % (group * groups);

	line_chars = group * 2 * groups + groups;
	ret += line_chars * full_lines;

	rem_groups = rem_bytes / group;
	ret += rem_groups * group * 2 + groups;

	rem_grp_bytes = rem_bytes - rem_groups * group;
	ret += rem_grp_bytes * 2;

	return ret;
}
