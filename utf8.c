#include <stdlib.h>
#include <stdio.h>

#include "gramas/utf8.h"

#define LEADING_ZEROES(num)	((~0 >> num) << num)
#define LEADING_ONES(num)	(~LEADING_ZEROES(num))
#define BETWEEN(num, from, to)		(((num) >= from) && ((num) <= to))

static int gr_ucs_utf8_len(gr_ucs_char ch);
static int gr_utf8_enc_bytes(char ch);
static void gr_utf8_next(const char **str);
static gr_ucs_char gr_utf8_read_char(const char **str);
static void gr_ucs_write_char(const gr_ucs_char **ucs_str, char **utf8_str);
static size_t gr_utf8_num_of_chars(const char *str);
static size_t gr_ucs_utf8_byte_len(const gr_ucs_char *str);

static inline int32_t get_bits(int32_t num, int from, int to)
{
	return (num >> from) & ~(~0 << (to - from + 1));
}

static inline void append_bits(int32_t *i, int32_t bits, int num)
{
	*i <<= num;
	*i |= bits & ((~0 >> num) << num);
}

gr_ucs_char * gr_utf8_to_ucs(const char *utf8_str)
{
	gr_ucs_char *ret;
	size_t len;
	size_t i;

	len = gr_utf8_num_of_chars(utf8_str);
	ret = malloc(sizeof(*ret) * (len + 1));

	for (i = 0; i < len; i++) {
		ret[i] = gr_utf8_read_char(&utf8_str);
	}
	ret[len] = 0;

	return ret;
}

char * gr_ucs_to_utf8(const gr_ucs_char *ucs_str)
{
	char *str;
	char *ret;
	size_t len;

	len = gr_ucs_utf8_byte_len(ucs_str);
	str = malloc(len + 1);
	ret = str;

	while (*ucs_str) {
		gr_ucs_write_char(&ucs_str, &str);
	}
	str[len] = '\0';

	return ret;
}

static int gr_ucs_utf8_len(gr_ucs_char ch)
{
	if (BETWEEN(ch, 0, 0x7F)) return 1;
	if (BETWEEN(ch, 0x80, 0x7FF)) return 2;
	if (BETWEEN(ch, 0x800, 0xFFFF)) return 3;
	if (BETWEEN(ch, 0x10000, 0x1FFFF)) return 4;
	return 0;
}

static int gr_utf8_enc_bytes(char ch)
{
	if ((ch & LEADING_ZEROES(3)) == LEADING_ZEROES(4)) {
		return 4;
	} else if ((ch & LEADING_ZEROES(4)) == LEADING_ZEROES(5)) {
		return 3;
	} else if ((ch & LEADING_ZEROES(5)) == LEADING_ZEROES(6)) {
		return 2;
	} else if ((ch & LEADING_ZEROES(7)) == 0) {
		return 1;
	}
	return 0; /* Error */
}

static void gr_utf8_next(const char **str)
{
	*str += gr_utf8_enc_bytes(**str);
}

static gr_ucs_char gr_utf8_read_char(const char **str)
{
	int i;
	int bytes;
	gr_ucs_char ret;

	ret = 0;
	bytes = gr_utf8_enc_bytes(**str);
	switch (bytes) {
		case 1:
			ret = **str;
			break;
		case 2:
			ret |= **str & LEADING_ONES(5);
			break;
		case 3:
			ret |= **str & LEADING_ONES(4);
			break;
		case 4:
			ret |= **str & LEADING_ONES(3);
			break;
		default:
			break;
	}
	*str += 1;

	for (i = 0; i < bytes - 1; i++) {
		ret <<= 6;
		ret |= **str & LEADING_ONES(6);
		*str += 1;
	}

	return ret;
}

static void gr_ucs_write_char(const gr_ucs_char **ucs_str, char **utf8_str)
{
	gr_ucs_char ch;

	ch = **ucs_str;
	if (BETWEEN(ch, 0, 0x7F)) {
		**utf8_str = (char)**ucs_str;
		*utf8_str += 1;
	} else if (BETWEEN(ch, 0x80, 0x7FF)) {
		**utf8_str = (char)(LEADING_ZEROES(6) | get_bits(ch, 6, 10));
		*utf8_str += 1;
		**utf8_str = (char)(0x80 | get_bits(ch, 0, 5));
		*utf8_str += 1;
	} else if (BETWEEN(ch, 0x800, 0xFFFF)) {
		**utf8_str = (char)(LEADING_ZEROES(5) | get_bits(ch, 12, 15));
		*utf8_str += 1;
		**utf8_str = (char)(0x80 | get_bits(ch, 6, 11));
		*utf8_str += 1;
		**utf8_str = (char)(0x80 | get_bits(ch, 0, 5));
		*utf8_str += 1;
	} else if (BETWEEN(ch, 0x10000, 0x1FFFF)) {
		**utf8_str = (char)(LEADING_ZEROES(4) | get_bits(ch, 18, 20));
		*utf8_str += 1;
		**utf8_str = (char)(0x80 | get_bits(ch, 12, 17));
		*utf8_str += 1;
		**utf8_str = (char)(0x80 | get_bits(ch, 6, 11));
		*utf8_str += 1;
		**utf8_str = (char)(0x80 | get_bits(ch, 0, 5));
		*utf8_str += 1;
	}
	*ucs_str += 1;
}

static size_t gr_utf8_num_of_chars(const char *str)
{
	size_t ret;
	int len;

	ret = 0;
	while (*str) {
		len = gr_utf8_enc_bytes(*str); if (!len) return 0;
		str += len;
		ret++;
	}

	return ret;
}

static size_t gr_ucs_utf8_byte_len(const gr_ucs_char *str)
{
	size_t ret;

	ret = 0;
	while (*str) {
		ret += gr_ucs_utf8_len(*str);
		str++;
	}

	return ret;
}
