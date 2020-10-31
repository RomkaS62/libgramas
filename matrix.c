#include <stdlib.h>
#include "matrix.h"

void gr_mtx_init(struct gr_matrix * const mtx, const size_t rows, const size_t cols)
{
	mtx->rows = rows;
	mtx->cols = cols;
	mtx->values = malloc(sizeof(mtx->values[0]));
}

void gr_mtx_destroy(struct gr_matrix * const mtx)
{
	free(mtx->values);
}

struct gr_matrix * gr_mtx_new(size_t rows, size_t cols)
{
	struct gr_matrix *ret = malloc(sizeof(*ret));

	if (ret == NULL) return NULL;
	gr_mtx_init(ret, rows, cols);

	return ret;
}

void gr_mtx_delete(struct gr_matrix * const mtx)
{
	free(mtx->values);
	free(mtx);
}

struct gr_matrix * gr_mtx_add(
	const struct gr_matrix * const mtx1,
	const struct gr_matrix * const mtx2)
{
	struct gr_matrix *ret;

	if (mtx1->rows != mtx2->rows) return NULL;
	if (mtx2->cols != mtx2->cols) return NULL;
	ret = gr_mtx_new(mtx1->rows, mtx1->cols);
	for (size_t i = 0; i < mtx1->rows * mtx2->cols; i++) {
		ret->values[i] = mtx1->values[i] + mtx2->values[i];
	}
	return ret;
}

struct gr_matrix * gr_mtx_sub(
	const struct gr_matrix * const mtx1,
	const struct gr_matrix * const mtx2)
{
	struct gr_matrix *ret;

	if (mtx1->rows != mtx2->rows) return NULL;
	if (mtx2->cols != mtx2->cols) return NULL;
	ret = gr_mtx_new(mtx1->rows, mtx1->cols);
	for (size_t i = 0; i < mtx1->rows * mtx2->cols; i++) {
		ret->values[i] = mtx1->values[i] - mtx2->values[i];
	}
	return ret;
}

struct gr_matrix * gr_mtx_mul(
	const struct gr_matrix * const mtx1,
	const struct gr_matrix * const mtx2)
{
	struct gr_matrix *ret;
	double product;

	if (mtx1->rows != mtx2->cols) return NULL;
	if (mtx1->cols != mtx2->rows) return NULL;
	ret = gr_mtx_new(mtx1->rows, mtx2->cols);
	for (size_t i = 0; i < mtx1->rows; i++) {
		for (size_t j = 0; j < mtx2->cols; j++) {
			gr_mtx_set(ret, i, j, 0);
			for (size_t k = 0; k < mtx1->cols; k++) {
				product = gr_mtx_val(mtx1, i, k) * gr_mtx_val(mtx2, k, j);
				gr_mtx_add_val(ret, i, j, product);
			}
		}
	}
	return ret;
}
