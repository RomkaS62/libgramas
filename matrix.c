#include <stdlib.h>
#include <math.h>
#include "gramas/matrix.h"

void gr_mtx_init(struct gr_matrix * const mtx, const size_t rows, const size_t cols)
{
	mtx->rows = rows;
	mtx->cols = cols;
	mtx->values = malloc(sizeof(mtx->values[0]) * rows * cols);
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
	if (mtx1->cols != mtx2->cols) return NULL;
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
	if (mtx1->cols != mtx2->cols) return NULL;
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

struct gr_matrix * gr_mtx_mul_tr_r(
	const struct gr_matrix * const mtx1,
	const struct gr_matrix * const mtx2)
{
	struct gr_matrix *ret;
	double product;

	if (mtx1->rows != mtx2->rows) return NULL;
	if (mtx1->cols != mtx2->cols) return NULL;
	ret = gr_mtx_new(mtx1->rows, mtx1->rows);
	for (size_t i = 0; i < mtx1->rows; i++) {
		for (size_t j = 0; j < mtx1->rows; j++) {
			gr_mtx_set(ret, i, j, 0.0);
			for (size_t k = 0; k < mtx1->cols; k++) {
				product = gr_mtx_val(mtx1, i, k) * gr_mtx_val(mtx2, i, k);
				gr_mtx_add_val(ret, i, j, product);
			}
		}
	}
	return ret;
}

struct gr_matrix * gr_mtx_row_sum(const struct gr_matrix * const mtx)
{
	struct gr_matrix *ret;
	double sum;

	ret = gr_mtx_new(mtx->rows, 1);
	for (size_t i = 0; i < mtx->rows; i++) {
		sum = 0.0;
		for (size_t j = 0; j < mtx->cols; j++) {
			sum += gr_mtx_val(mtx, i, j);
		}
		gr_mtx_set(ret, i, 0, sum);
	}

	return ret;
}

void gr_mtx_square_m(struct gr_matrix * const mtx)
{
	if (mtx == NULL) return;
	for (size_t i = 0; i < mtx->rows * mtx->cols; i++) {
		mtx->values[i] *= mtx->values[i];
	}
}

void gr_mtx_sqrt_m(struct gr_matrix * const mtx)
{
	if (mtx == NULL) return;
	for (size_t i = 0; i < mtx->rows * mtx->cols; i++) {
		mtx->values[i] = sqrt(mtx->values[i]);
	}
}
