#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>
#include "field.h"
#include "str.h"

static struct test_read_s {
	char buf[sizeof(uint64_t)];
	uint64_t value;
	size_t len;
	int big_endian;
} read_cases[] = {
	{
		{1, 2, 3},	0x030201,	3, 0
	}, {
		{4, 2, 1},	0x010204,	3, 0
	}, {
		{1, 2},		0x0201,		2, 0
	}, {
		{1, 2, 3},	0x010203,	3, 1
	}, {
		{4, 2, 1},	0x040201,	3, 1
	}
};

static struct test_write_s {
	uint64_t value;
	size_t len;
	char buf[sizeof(uint64_t)];
	int big_endian;
} write_cases[] = {
	{
		0x0102, 2, {1, 2}, 1
	}, {
		0x040203, 3, {4, 2, 3}, 1
	}, {
		0x060504, 3, {4, 5, 6}, 0
	}, {
		0x040201, 3, {1, 2, 4}, 0
	}
};

int main(void)
{
	int ret = 0;
	size_t i;
	uint64_t value;
	char buf[sizeof(uint64_t)];
	char *hexbuf1 = NULL;
	char *hexbuf2 = NULL;
	struct test_read_s *rc;
	struct test_write_s *wc;

	for (i = 0; i < sizeof(read_cases) / sizeof(read_cases[0]); i++) {
		rc = &read_cases[i];
		if (rc->big_endian) {
			if (rc->value != (value = read_be(rc->buf, rc->len)))
				goto fail_rd;
		} else {
			if (rc->value != (value = read_le(rc->buf, rc->len)))
				goto fail_rd;
		}
	}

	for (i = 0; i < sizeof(write_cases) / sizeof(write_cases[0]); i++) {
		memset(buf, 0, sizeof(buf));
		wc = &write_cases[i];

		if (wc->big_endian)
			write_be(wc->value, buf, wc->len);
		else
			write_le(wc->value, buf, wc->len);

		if (memcmp(buf, wc->buf, wc->len)) {
			if (wc->big_endian)
				goto fail_wr;
			else
				goto fail_wr;
		}
	}

	goto end;

fail_rd:
	gr_buf2hex(&hexbuf1, rc->buf, rc->len);
	puts("\nRead failed!");
	printf(
		"  Given buffer: %s\n"
		"     Endianess: %s\n"
		"Expected value: %016"PRIx64"\n"
		"  Actual value: %016"PRIx64"\n"
		,	hexbuf1
		,	(rc->big_endian) ? "Big-endian" : "Little-endian"
		,	rc->value
		,	value
	);
	ret = 1;
	goto end;

fail_wr:
	gr_buf2hex(&hexbuf1, wc->buf, wc->len);
	gr_buf2hex(&hexbuf2, buf, wc->len);
	puts("\nWrite failed!");
	printf(
		"          Value: %016"PRIx64"\n"
		"      Endianess: %s\n"
		"Expected buffer: %s\n"
		"  Actual buffer: %s\n"
		,	wc->value
		,	(wc->big_endian) ? "Big-endian" : "Little-endian"
		,	hexbuf1
		,	hexbuf2
	);
	ret = 1;
	goto end;

end:
	free(hexbuf1);
	free(hexbuf2);
	return ret;
}
