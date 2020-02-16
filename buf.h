#ifndef LIBUTIL_BUF_H
#define LIBUTIL_BUF_H

#include <stdlib.h>

/* Members are """private""". Do not touch with your dirty hands! */
struct buffer {
	void *buf;		/* Backing buffer */
	size_t capacity;	/* Capacity in members */
	size_t used;		/* Members present */
	size_t member_size;	/* Size of a member in bytes */
};

#define buf_at(b, idx) ((b)->buf + (b)->member_size * (idx))
#define buf_set(buf_ptr, at, ptr) __buf_set(buf_ptr, at, ptr, sizeof(*ptr))
#define buf_len(buf_ptr) ((buf_ptr)->used)
/* buf_ptr - struct buffer *.
 * var - pointer to whatever it is that you store in the buffer.
 * tmp - an identifier that you promise not to use.
 * */
#define buf_foreach(buf_ptr, var, tmp)				\
	for (size_t tmp = 0;					\
		(tmp < (buf_ptr)->used)				\
			? var = buf_at((buf_ptr), tmp), 1	\
			: 0;					\
		tmp++)

void buf_init(struct buffer *buf, size_t member_count, size_t member_size);
void buf_delete(struct buffer *buf);
void buf_free(struct buffer *buf);
void buf_append(struct buffer *buf, void *ptr);
void __buf_set(struct buffer *buf, size_t at, void *ptr, size_t size);
void buf_remove_range(struct buffer *buf, size_t from, size_t to);
/* Resizes internal buffer to fit the amount of elements actually stored. */
void buf_trim(struct buffer *buf);

#endif /* LIBUTIL_BUF_H */
