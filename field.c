#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gramas/field.h"
#include "gramas/packet.h"

size_t gr_field_write(
		const struct gr_field *field,
		char *buf,
		size_t len,
		enum gr_pck_parse_state *err)
{
	uint64_t field_len;

	field_len = gr_field_length(field);

	if (field_len > len) {
		*err = GR_PCK_UNEXPECTED_END;
		return 0;
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
			*err = GR_PCK_FAIL;
			return 0;
		}
		memcpy(buf, field->var_len_field.bytes, field_len);
		goto success;

	case GR_FIELD_PACKET:
		field_len = gr_pck_read(field->packet, buf, len, err);
		goto success;

	case GR_FIELD_USR:
		return field->usr_field.write(buf, len, err, field->usr_field.usr_data);

	default:
		fputs("gr_field_write(): invalid tag!", stderr);
		exit(1);
		return 0;
	}

success:
	*err = GR_PCK_SUCCESS;
	return (long)field_len;
}

size_t gr_field_read(
		struct gr_field *field,
		const char *buf,
		size_t len,
		enum gr_pck_parse_state *err)
{
	size_t field_len;

	field_len = gr_field_length(field);

	if (field_len > len) {
		*err = GR_PCK_UNEXPECTED_END;
		return 0;
	}

	switch (field->ftype) {
	case GR_FIELD_CONST:
		if (memcmp(field->const_field.bytes, buf, field->const_field.length)) {
			*err = GR_PCK_FAIL;
			return 0;
		}
		goto success;

	case GR_FIELD_INT:
		field->int_field.value = (field->int_field.big_endian)
			? read_be(buf, field_len)
			: read_le(buf, field_len);

		goto success;

	case GR_FIELD_VAR_LENGTH:
		if (field->var_len_field.len_field->ftype != GR_FIELD_INT) {
			*err = GR_PCK_FAIL;
			return 0;
		}
		memcpy(field->var_len_field.bytes, buf, field_len);
		goto success;

	case GR_FIELD_PACKET:
		field_len = gr_pck_read(field->packet, buf, len, err);
		if (*err == GR_PCK_SUCCESS)
			goto success;
		return 0;

	case GR_FIELD_USR:
		return field->usr_field.read(buf, len, err, field->usr_field.usr_data);

	default:
		fputs("gr_field_write(): invalid tag!", stderr);
		exit(1);
		return -1;
	}

success:
	*err = GR_PCK_SUCCESS;
	if (field->on_read)
		field->on_read(field, field->data);
	return (long)field_len;
}

size_t gr_field_length(const struct gr_field *field)
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
	case GR_FIELD_PACKET:
		return 0;
	default:
		fputs("gr_field_length(): invalid tag", stderr);
		exit(1);
		return 1;
	}
}
