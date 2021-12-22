#ifndef LIBGRAMASL_BUF_H
#define LIBGRAMASL_BUF_H

#include <stdlib.h>
#include "gramas/gr_export.h"

GR_CDECL_BEGIN

/* Members are """private""". Do not touch with your dirty hands! */
struct GR_EXPORT buffer {
	char *buf;		/* Backing buffer		*/
	size_t capacity;	/* Capacity in members		*/
	size_t used;		/* Members present		*/
	size_t member_size;	/* Size of a member in bytes	*/
#ifndef NDEBUG
	int _untinitialized;
#endif
};

#define buf_at(b, idx) ((b)->buf + (b)->member_size * (idx))
#define buf_set(buf_ptr, at, ptr) __buf_set(buf_ptr, at, ptr, sizeof(*ptr))
#define buf_len(buf_ptr) ((buf_ptr)->used)
/* buf_ptr - struct buffer *.
 * var - pointer to whatever it is that you store in the buffer.
 * idx - index of the element.
 * */
#define buf_foreach(buf_ptr, var, idx)				\
	for (size_t idx = 0;					\
		(idx < (buf_ptr)->used)				\
			? var = buf_at((buf_ptr), idx), 1	\
			: 0;					\
		idx++)

typedef size_t (*read_fn)(char *dest, size_t membsz, size_t amount, void *src);

GR_EXPORT void buf_init(struct buffer *buf, size_t member_count, size_t member_size);
GR_EXPORT void buf_delete(struct buffer *buf);
GR_EXPORT void buf_free(struct buffer *buf);
GR_EXPORT void buf_append(struct buffer *buf, void *ptr);
GR_EXPORT void __buf_set(struct buffer *buf, size_t at, void *ptr, size_t size);
GR_EXPORT void buf_remove_range(struct buffer *buf, size_t from, size_t to);
/* Resizes internal buffer to fit the amount of elements actually stored. */
GR_EXPORT void buf_trim(struct buffer *buf);
GR_EXPORT void buf_clear(struct buffer *buf);
GR_EXPORT void buf_append_arr(struct buffer *buf, const void *arr, const size_t members);
GR_EXPORT size_t buf_read_f(struct buffer *buf, FILE *file, size_t amount);
GR_EXPORT size_t buf_read(
		struct buffer *buf,
		void *src,
		size_t amount,
		read_fn read);

GR_CDECL_END

#endif /* LIBGRAMAS_BUF_H */
