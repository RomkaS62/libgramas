#ifndef LIBGRAMAS_SPACE_H
#define LIBGRAMAS_SPACE_H

#include "matrix.h"

struct gr_matrix * gr_spc_distance(
		const struct gr_matrix * const pts1,
		const struct gr_matrix * const pts2);

#endif /* LIBGRAMAS_SPACE_H */
