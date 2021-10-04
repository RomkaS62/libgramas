#include <stdlib.h>
#include "gramas/field.h"
#include "gramas/packet.h"

size_t gr_pck_read(
		struct gr_packet *packet,
		const char *buf,
		size_t len,
		enum gr_pck_parse_state *state)
{
	size_t i;
	size_t bytes_read;
	size_t ret = 0;

	for (i = 0; i < packet->len; i++) {
		bytes_read = gr_field_read(&packet->fields[i],
				buf + ret, len - ret,  state);
		if (*state != GR_PCK_SUCCESS)
			break;
		ret += bytes_read;
	}

	return ret;
}

size_t gr_pck_write(
		const struct gr_packet *packet,
		char *buf,
		size_t len,
		enum gr_pck_parse_state *state)
{
	size_t i;
	size_t bytes_written;
	size_t ret = 0;

	for (i = 0; i < packet->len; i++) {
		bytes_written = gr_field_write(&packet->fields[i],
				buf + ret, len - ret, state);
		if (*state != GR_PCK_SUCCESS)
			return 0;
		ret += bytes_written;
	}

	return ret;
}
