#ifndef LIBGRAMAS_STR_H
#define LIBGRAMAS_STR_H

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
	return (nib > 10)
		? nib + 'A' - 10
		: nib + '0';
}

void gr_buf2hex(char **str, const char *data, size_t len);

#endif /* LIBGRAMAS_STR_H */
