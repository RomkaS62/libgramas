#ifndef LIBGRAMAS_MEMDBG_H
#define LIBGRAMAS_MEMDBG_H

#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG
static inline void __gr_memdbg_null_check(void *ptr, const char *func)
{
	if (ptr == NULL) {
		fprintf(stderr, "[%s] Unexpeced NULL pointer!\n", func);
		exit(1);
	}
}

#define gr_null_check(__ptr) __gr_memdbg_null_check((void *)__ptr, __func__)
#else /* Release */
#define gr_null_check(__ptr)
#endif /* Release */

#endif /* LIBGRAMAS_MEMDBG_H */
