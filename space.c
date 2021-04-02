#include <math.h>
#include "space.h"

struct gr_matrix * gr_spc_distance(
		const struct gr_matrix * const pts1,
		const struct gr_matrix * const pts2)
{
	struct gr_matrix *diff;
	struct gr_matrix *ret;

	if (pts1 == NULL) return NULL;
	if (pts2 == NULL) return NULL;
	if (pts1->rows != pts2->rows || pts1->cols != pts2->cols) return NULL;

	diff = gr_mtx_sub(pts2, pts1);
	gr_mtx_square_m(diff);
	ret = gr_mtx_row_sum(diff);
	gr_mtx_sqrt_m(ret);

	gr_mtx_delete(diff);

	return ret;
}
