#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
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

size_t gr_str_read_until(const char *str, char ch, char *dest)
{
	size_t ret = 0;

	if (str == NULL || dest == NULL)
		return 0;

	while (*str) {
		if (*str == ch)
			break;

		*dest = *str;
		str++;
		dest++;
		ret++;
	}
	*dest = '\0';

	return ret;
}

size_t gr_str_split_pv(const char *str, char ch, size_t parts, va_list args)
{
	size_t ret = 0;
	size_t chars_read = 0;
	size_t total_chars_read = 0;
	char *dest;

	if (parts == 0 || str == NULL)
		return 0;

	dest = va_arg(args, char *);

	while ((chars_read = gr_str_read_until(str + total_chars_read, ch, dest)) != 0) {
		total_chars_read += chars_read;
		ret++;
		if (!str[total_chars_read]) {
			break;
		}
		dest = va_arg(args, char *);
		total_chars_read++;
	}

	return ret;
}

size_t gr_str_split_p(const char *str, char ch, size_t parts, ...)
{
	va_list args;
	size_t ret;

	va_start(args, parts);
	ret = gr_str_split_pv(str, ch, parts, args);
	va_end(args);

	return ret;
}

size_t gr_str_chars_until(const char *str, char ch)
{
	size_t ret = 0;

	while (*str) {
		if (*str != ch) {
			ret++;
			str++;
		} else {
			break;
		}
	}

	return ret;
}

size_t gr_str_num_of_char(const char *str, char ch)
{
	size_t ret = 0;

	while (*str) {
		if (*str == ch)
			ret++;
		str++;
	}

	return ret;
}

struct str_array gr_str_split(const char *str, char ch)
{
	struct str_array ret = {0, NULL};
	size_t i = 0;
	size_t part_length = 0;
	size_t total_read = 0;
	const char *read_from = str;
	char *part = NULL;

	ret.length = gr_str_num_of_char(str, ch) + 1;
	ret.strings = malloc(sizeof(ret.strings[0]) * ret.length);

	for (i = 0; i < ret.length; i++) {
		read_from = str + total_read;
		part_length = gr_str_chars_until(read_from, ch);
		part = malloc(part_length + 1);
		memcpy(part, read_from, part_length);
		part[part_length] = '\0';
		total_read += part_length + 1;
		ret.strings[i] = part;
	}

	return ret;
}
