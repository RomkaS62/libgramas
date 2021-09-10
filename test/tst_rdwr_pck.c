#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "field.h"
#include "packet.h"

static struct gr_packet packet = {
	(struct gr_field[]) {
		{
			.ftype = GR_FIELD_CONST,
			.const_field = {
				.bytes = (char[]){1, 2, 3},
				.length = 3
			}
		}, {
			.ftype = GR_FIELD_INT,
			.int_field = {
				.value = 0,
				.length = 3,
				.big_endian = 1
			}
		}, {
			.ftype = GR_FIELD_INT,
			.int_field = {
				.value = 0,
				.length = 2,
				.big_endian = 1
			}
		}, {
			.ftype = GR_FIELD_VAR_LENGTH,
			.var_len_field = {
				.len_field = NULL,
				.bytes = (char[]){0, 0, 0}
			}
		}, {
			.ftype = GR_FIELD_CONST,
			.const_field = {
				.bytes = (char[]){3, 2, 1},
				.length = 3
			}
		}
	},
	5
};

const char data[] = {
	1, 2, 3,
	3, 2, 1,
	0, 3,
	5, 6, 7,
	3, 2, 1
};

int main(void)
{
	enum gr_pck_parse_state state;

	packet.fields[3].var_len_field.len_field = &packet.fields[2];

	gr_pck_read(&packet, data, sizeof(data), &state);
	assert(state == GR_PCK_SUCCESS);
	assert(packet.fields[1].int_field.value == 0x030201);
	assert(packet.fields[2].int_field.value == 3);
	assert(memcmp(packet.fields[3].var_len_field.bytes, data + 8, 3) == 0);

	return 0;
}
