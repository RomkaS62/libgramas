#ifndef LIBGRAMAS_STR_H
#define LIBGRAMAS_STR_H

#include <stdlib.h>
#include "gramas/gr_export.h"

GR_CDECL_BEGIN

struct GR_EXPORT gr_str {
	char *str;
	size_t length;	/* Includes terminator */
};

GR_EXPORT void gr_str_init(struct gr_str *str);
/**
 * Frees the backing string.
 */
GR_EXPORT void gr_str_delete(struct gr_str *str);
/**
 * Frees the backing string and struct itself.
 */
GR_EXPORT void gr_str_free(struct gr_str *str);
/**
 * Replaces backing string with output from sprintf.
 */
GR_EXPORT void gr_sprintf(struct gr_str *str, const char *fmt, ...);
/**
 * Appends text to string with output from sprintf.
 */
GR_EXPORT void gr_sprintf_a(struct gr_str *str, const char *fmt, ...);

GR_CDECL_END

#endif /* LIBGRAMAS_STR_H */
