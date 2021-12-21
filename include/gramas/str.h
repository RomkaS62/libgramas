#ifndef LIBGRAMAS_STR_H
#define LIBGRAMAS_STR_H

#include <stdarg.h>
#include <stdlib.h>

static inline char gr_hinib(char ch)
{
	return (ch >> 4) & 0xF;
}

static inline char gr_lonib(char ch)
{
	return ch & 0xF;
}

static inline char gr_nib2hex(char nib)
{
	return (nib >= 10)
		? nib + 'A' - 10
		: nib + '0';
}

void gr_buf2hex(char **str, const char *data, size_t len);
void gr_hexdump(const char *data,
		size_t len,
		char *str,
		size_t group,
		size_t groups);
size_t gr_hexdump_len(size_t data_len, size_t group, size_t groups);

size_t gr_str_read_until(const char *str, char ch, char *dest);
size_t gr_str_split_pv(const char *str, char ch, size_t parts, va_list args);
size_t gr_str_split_p(const char *str, char ch, size_t parts, ...);

size_t gr_str_chars_until(const char *str, char ch);
size_t gr_str_num_of_char(const char *str, char ch);

struct str_array {
	size_t length;
	char **strings;
};

static inline void gr_str_array_delete(struct str_array *arr)
{
	size_t i;

	if (arr == NULL)
		return;

	for (i = 0; i < arr->length; i++) {
		free(arr->strings[i]);
	}

	free(arr->strings);
}

struct str_array gr_str_split(const char *str, char ch);

#endif /* LIBGRAMAS_STR_H */
