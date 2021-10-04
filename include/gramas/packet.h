#ifndef LIBGRAMAS_PACKET_H
#define LIBGRAMAS_PACKET_H

#include "gramas/field.h"

struct gr_packet {
	struct gr_field *fields;
	size_t len;
};

size_t gr_pck_read(
		struct gr_packet *packet,
		const char *buf,
		size_t len,
		enum gr_pck_parse_state *state);
size_t gr_pck_write(
		const struct gr_packet *packet,
		char *buf,
		size_t len,
		enum gr_pck_parse_state *state);

#endif /* LIBGRAMAS_PACKET_H */
