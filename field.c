#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "field.h"

long gr_field_write(
		const struct gr_field *field,
		char *buf,
		size_t len,
		enum gr_pck_parse_state *err)
{
	uint64_t field_len;

	field_len = gr_field_length(field);

	if (field_len > len) {
		*err = GR_PCK_UNEXPECTED_END;
		return -1;
	}

	switch (field->ftype) {
	case GR_FIELD_CONST:
		memcpy(buf, field->const_field.bytes, field->const_field.length);
		goto success;

	case GR_FIELD_INT:
		if (field->int_field.big_endian) {
			write_be(field->int_field.value, buf, len);
		} else {
			write_le(field->int_field.value, buf, len);
		}
		goto success;

	case GR_FIELD_VAR_LENGTH:
		if (field->var_len_field.len_field->ftype != GR_FIELD_INT) {
			return -1;
		}
		field_len = field->int_field.value;
		memcpy(buf, field->var_len_field.bytes, field_len);
		goto success;

	case GR_FIELD_USR:
		return field->usr_field.write(buf, len, err, field->usr_field.usr_data);

	default:
		fputs("gr_field_write(): invalid tag!", stderr);
		exit(1);
		return -1;
	}

success:
	*err = GR_PCK_SUCCESS;
	return (long)field_len;
}

long gr_field_read(
		struct gr_field *field,
		const char *buf,
		size_t len,
		enum gr_pck_parse_state *err)
{
	size_t field_len;

	field_len = gr_field_length(field);

	if (field_len > len) {
		*err = GR_PCK_UNEXPECTED_END;
		return -1;
	}

	switch (field->ftype) {
	case GR_FIELD_CONST:
		if (memcmp(field->const_field.bytes, buf, field->const_field.length)) {
			*err = GR_PCK_FAIL;
			return -1;
		}
		goto success;

	case GR_FIELD_INT:
		field->int_field.value = (field->int_field.big_endian)
			? read_be(buf, len)
			: read_le(buf, len);

		goto success;

	case GR_FIELD_VAR_LENGTH:
		memcpy(field->var_len_field.bytes, buf, field->var_len_field.length);
		goto success;

	case GR_FIELD_USR:
		return field->usr_field.read(buf, len, err, field->usr_field.usr_data);

	default:
		fputs("gr_field_write(): invalid tag!", stderr);
		exit(1);
		return -1;
	}

success:
	*err = GR_PCK_SUCCESS;
	return (long)field_len;
}
