#ifndef LIBGRAMAS_MATRIX_H
#define LIBGRAMAS_MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include "gr_export.h"

#define for_each_value(matrix_ptr, value, idx) \
	for (size_t idx = 0; idx < (matrix_ptr)->rows * (matrix_ptr)->cols; idx++)

#define for_each_row(matrix_ptr, row, idx) \
	for (size_t idx = 0;	\
			(idx < (matrix_ptr)->rows)	\
				?	row = (matrix_ptr)->values + idx * (matrix_ptr)->cols, 1\
				:	row = NULL, 0	\
			; idx++)

GR_CDECL_BEGIN

struct GR_EXPORT gr_matrix {
	size_t rows;
	size_t cols;
	double *values;
};

GR_EXPORT void gr_mtx_init(struct gr_matrix * const mtx, const size_t rows, const size_t cols);
GR_EXPORT void gr_mtx_destroy(struct gr_matrix * const mtx);
GR_EXPORT struct gr_matrix * gr_mtx_new(size_t rows, size_t cols);
GR_EXPORT void gr_mtx_delete(struct gr_matrix * const mtx);

GR_EXPORT struct gr_matrix * gr_mtx_add(
	const struct gr_matrix * const mtx1,
	const struct gr_matrix * const mtx2);

GR_EXPORT struct gr_matrix * gr_mtx_sub(
	const struct gr_matrix * const mtx1,
	const struct gr_matrix * const mtx2);

/* Basic matrix multiplication */
GR_EXPORT struct gr_matrix * gr_mtx_mul(
	const struct gr_matrix * const mtx1,
	const struct gr_matrix * const mtx2);

/* Matrix multiplication with the right matrix transposed. */
GR_EXPORT struct gr_matrix * gr_mtx_mul_tr_r(
	const struct gr_matrix * const mtx1,
	const struct gr_matrix * const mtx2);

GR_EXPORT struct gr_matrix * gr_mtx_row_sum(const struct gr_matrix * const mtx);

/* Matrix operations that modify first operand */

GR_EXPORT void gr_mtx_square_m(struct gr_matrix * const mtx);
GR_EXPORT void gr_mtx_sqrt_m(struct gr_matrix * const mtx);

#define _CHECK_BOUNDS(mtx_ptr, row, col)	\
	do {	\
		if (row > mtx->rows) {	\
			fputs("Matrix row out of bounds!", stderr);	\
			return;	\
		}	\
		if (col > mtx->cols) {	\
			fputs("Matrix column out of bounds!", stderr);	\
			return;	\
		}	\
	} while (0)

#define _CHECK_BOUNDS_RET(mtx_ptr, row, col, val)	\
	do {	\
		if (row > mtx->rows) {	\
			fputs("Matrix row out of bounds!", stderr);	\
			return val;	\
		}	\
		if (col > mtx->cols) {	\
			fputs("Matrix column out of bounds!", stderr);	\
			return val;	\
		}	\
	} while (0)

static inline void gr_mtx_set(struct gr_matrix *mtx, size_t row, size_t col, double val)
{
	_CHECK_BOUNDS(mtx, row, col);
	mtx->values[mtx->cols * row + col] = val;
}

static inline void gr_mtx_add_val(struct gr_matrix *mtx, size_t row, size_t col, double val)
{
	_CHECK_BOUNDS(mtx, row, col);
	mtx->values[mtx->cols * row + col] += val;
}

static inline double gr_mtx_val(const struct gr_matrix *mtx, size_t row, size_t col)
{
	_CHECK_BOUNDS_RET(mtx, row, col, 0.0);
	return mtx->values[mtx->cols * row + col];
}

GR_CDECL_END

#endif /* LIBGRAMAS_MATRIX_H */
