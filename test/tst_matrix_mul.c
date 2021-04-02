#include <stdlib.h>
#include "matrix.h"

static double a_values[] = {
	1.0,	2.0,	3.0,
	4.0,	5.0,	6.0,
	7.0,	8.0,	9.0
};

static struct gr_matrix a = {
	.rows = 3,
	.cols = 3,
	.values = a_values
};

static double b_values[] = {
	1.0,	1.0,	1.0,
	2.0,	2.0,	2.0,
	3.0,	3.0,	3.0
};

static struct gr_matrix b = {
	.rows = 3,
	.cols = 3,
	.values = b_values
};

static double correct_values[] = {
	14.0,	14.0,	14.0,
	32.0,	32.0,	32.0,
	50.0,	50.0,	50.0
};

int main(int argc, char **argv)
{
	int ret = 0;
	int i;
	struct gr_matrix *c;

	c = gr_mtx_mul(&a, &b);
	for (i = 0; i < 9; i++) {
		if (c->values[i] != correct_values[i]) {
			goto fail;
		}
	}
	goto end;

fail:
	ret = 1;
end:
	gr_mtx_delete(c);
	return ret;
}
