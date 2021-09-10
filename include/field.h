#ifndef LIBGRAMAS_FIELD_H
#define LIBGRAMAS_FIELD_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "str.h"

enum gr_pck_parse_state {
	GR_PCK_SUCCESS,
	GR_PCK_UNEXPECTED_END,
	GR_PCK_FAIL
};

enum gr_field_type {
	GR_FIELD_CONST,
	GR_FIELD_INT,
	GR_FIELD_VAR_LENGTH,
	GR_FIELD_USR
};

struct gr_field_usr {
	long (*write)(
			char *buf,
			size_t len,
			enum gr_pck_parse_state *err,
			void *usr_data);
	long (*read)(
			const char *buf,
			size_t len,
			enum gr_pck_parse_state *err,
			void *usr_data);
	size_t (*length)(void *usr_data);
	void *usr_data;
};

struct gr_field {
	enum gr_field_type ftype;
	union {
		struct {
			char *bytes;
			unsigned length;
		} const_field;
		struct {
			uint64_t value;
			unsigned char length;
			unsigned char big_endian;
		} int_field;
		struct {
			struct gr_field *len_field;
			char *bytes;
		} var_len_field;
		struct gr_field_usr usr_field;
	};
};

size_t gr_field_write(
		const struct gr_field *field,
		char *buf,
		size_t len,
		enum gr_pck_parse_state *err);

size_t gr_field_read(
		struct gr_field *field,
		const char *buf,
		size_t len,
		enum gr_pck_parse_state *err);

static inline size_t gr_field_length(const struct gr_field *field)
{
	switch (field->ftype) {
	case GR_FIELD_CONST:
		return field->const_field.length;
	case GR_FIELD_INT:
		return field->int_field.length;
	case GR_FIELD_VAR_LENGTH:
		return field->var_len_field.len_field->int_field.value;
	case GR_FIELD_USR:
		return field->usr_field.length(field->usr_field.usr_data);
	default:
		fputs("gr_field_length(): invalid tag", stderr);
		exit(1);
		return 1;
	}
}

static inline uint64_t read_be(const char *buf, size_t int_len)
{
	uint64_t ret = 0;
	while (int_len > 0) {
		ret <<= 8;
		ret |= *buf++;
		int_len--;
	}

	return ret;
}

static inline uint64_t read_le(const char *buf, size_t int_len)
{
	uint64_t ret = 0;
	size_t i;

	for (i = 0; i < int_len; i++)
		ret |= (uint64_t)*buf++ << (i * 8);

	return ret;
}

static inline void write_be(uint64_t val, char *buf, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		*buf++ = (val >> ((len - i - 1) * 8)) & 0xFF;
}

static inline void write_le(uint64_t val, char *buf, size_t len)
{
	while (len > 0) {
		*buf++ = val & 0xFF;
		val >>= 8;
		len--;
	}
}

#endif /* LIBGRAMAS_FIELD_H */
