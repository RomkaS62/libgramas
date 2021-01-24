#ifndef LIBGRAMAS_UTF8_H
#define LIBGRAMAS_UTF8_H

#include <stdint.h>

typedef int32_t gr_ucs_char;

gr_ucs_char * gr_utf8_to_ucs(const char *utf8_str);
char * gr_ucs_to_utf8(const gr_ucs_char *ucs_str);

#endif /* LIBGRAMAS_UTF8_H */
