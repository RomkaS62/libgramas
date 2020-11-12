#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "gr_str.h"

void gr_str_init(struct gr_str *str)
{
	str->length = 0;
	str->str = NULL;
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
	int chars_needed;
	va_list args;
	
	va_start(args, fmt);
	chars_needed = vsnprintf(str->str, str->length, fmt, args);
	va_end(args);
	if (chars_needed >= str->length) {
		realloc(str->str, chars_needed + 1);
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
	chars_needed = vsnprintf(str->str + str->length - 1, 0, fmt, args);
	va_end(args);
	str->str = realloc(str->str, str->length + chars_needed);
	va_start(args, fmt);
	vsnprintf(str->str + str->length - 1, 0, fmt, args);
	va_end(args);
}