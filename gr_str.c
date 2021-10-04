#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "gramas/gr_str.h"

void gr_str_init(struct gr_str *str)
{
	str->length = 1;
	str->str = malloc(1);
	str->str[0] = '\0';
}

void gr_str_delete(struct gr_str *str)
{
	free(str->str);
}

void gr_str_free(struct gr_str *str)
{
	gr_str_delete(str);
	free(str);
}

void gr_sprintf(struct gr_str *str, const char *fmt, ...)
{
	int vsnp_ret;
	size_t chars_needed;
	va_list args;
	
	va_start(args, fmt);
	vsnp_ret = vsnprintf(str->str, str->length, fmt, args);
	if (vsnp_ret < 0) return;
	chars_needed = (size_t)vsnp_ret;
	va_end(args);
	if (chars_needed >= str->length) {
		str->str = realloc(str->str, chars_needed + 1);
		str->length = chars_needed + 1;
		va_start(args, fmt);
		vsnprintf(str->str, str->length, fmt, args);
		va_end(args);
	}
}

void gr_sprintf_a(struct gr_str *str, const char *fmt, ...)
{
	int chars_needed;
	va_list args;

	va_start(args, fmt);
	chars_needed = vsnprintf(str->str, 0, fmt, args) + 1;
	va_end(args);
	str->str = realloc(str->str, str->length + chars_needed);
	va_start(args, fmt);
	vsnprintf(str->str + str->length - 1, chars_needed, fmt, args);
	str->length += chars_needed - 1;
	va_end(args);
}
