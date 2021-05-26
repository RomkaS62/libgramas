#ifndef LIBGRAMAS_STR_H
#define LIBGRAMAS_STR_H

#include <stdlib.h>

struct gr_str {
	char *str;
	size_t length;	/* Includes terminator */
};

void gr_str_init(struct gr_str *str);
/**
 * Frees the backing string.
 */
void gr_str_delete(struct gr_str *str);
/**
 * Frees the backing string and struct itself.
 */
void gr_str_free(struct gr_str *str);
/**
 * Replaces backing string with output from sprintf.
 */
void gr_sprintf(struct gr_str *str, const char *fmt, ...);
/**
 * Appends text to string with output from sprintf.
 */
void gr_sprintf_a(struct gr_str *str, const char *fmt, ...);

#endif /* LIBGRAMAS_STR_H */