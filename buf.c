#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gramas/buf.h"
#include "gramas/memdbg.h"

#define GROW_FACTOR	(2)
#define BYTE_COUNT(b, from, to) ((b)->member_size * ((to) - (from)))

#ifndef NDEBUG
static void __buf_null_check(const struct buffer *buf, const char *func)
{
	if (buf == NULL) {
		fprintf(stderr, "[%s] Attempted use of NULL buffer.\n", func);
		exit(1);
	}

	if (buf->buf == NULL) {
		fprintf(stderr, "[%s] Buffer structure allocated but internal buffer is NULL!\n",
				func);
		exit(1);
	}
}

static void __buf_sanity_check(const struct buffer *buf, const char *func)
{
	if (buf->capacity == 0) {
		fprintf(stderr, "[%s] Zero capcity buffer!\n", func);
		exit(1);
	}

	if (buf->used > buf->capacity) {
		fprintf(stderr, "[%s] Buffer use greater than capacity!\n",
				func);
		exit(1);
	}

	if (buf->member_size == 0) {
		fprintf(stderr, "[%s] Zero member size buffer used!\n", func);
		exit(1);
	}
}

static void __buf_init_check(const struct buffer *buf, const char *func)
{
	if (buf->_untinitialized) {
		fprintf(stderr, "[%s] Unitialized buffer used!\n", func);
		exit(1);
	}
}

static void __buf_check(const struct buffer *buf, const char *func)
{
	__buf_null_check(buf, func);
	__buf_init_check(buf, func);
	__buf_sanity_check(buf, func);
}

static void __buf_free_check(const struct buffer *buf, const char *func)
{
	__buf_init_check(buf, func);
	__buf_sanity_check(buf, func);
}

#define buf_check(__buf) __buf_check(__buf, __func__)
#define buf_free_check(__buf) __buf_free_check(__buf, __func__)
#else /* Release */
#define buf_check(__buf)
#define buf_free_check(__buf)
#endif /* Release */

static const char ZERO_CAPACITY[] =
"Zero length buffers are not allowed!\n";

static const char ZERO_SIZE[] =
"Cannot store elements of zero length!\n";

static const char WRONG_SIZE[] =
"Attempting to insert member into buffer with incompatible size!\n";

static const char OUT_OF_BOUNDS[] =
"Attempting to insert member out of bounds!\n";

static void buf_ensure_capacity(struct buffer *buf, size_t capacity);
static void buf_grow(struct buffer *buf);
/**
 * Returns pointer to first unused byte in buffer.
 */
static void * buf_end_ptr(const struct buffer * const buf);

void buf_init(struct buffer *buf, size_t member_count, size_t member_size)
{
	if (member_size == 0) {
		fputs(ZERO_SIZE, stderr);
		return;
	}
	if (member_count == 0) {
		fputs(ZERO_CAPACITY, stderr);
		return;
	}
	buf->buf = malloc(member_size * member_count);
	buf->capacity = member_count;
	buf->used = 0;
	buf->member_size = member_size;
#ifndef NDEBUG
	buf->_untinitialized = 0;
#endif
}

void buf_delete(struct buffer *buf)
{
	buf_check(buf);
	free(buf->buf);
}

void buf_free(struct buffer *buf)
{
	if (buf == NULL)
		return;

	buf_free_check(buf);
	free(buf);
}

void buf_append(struct buffer *buf, void *ptr)
{
	gr_null_check(ptr);
	buf_check(buf);

	buf_ensure_capacity(buf, buf->used + 1);
	memcpy(buf_at(buf, buf->used), ptr, buf->member_size);
	buf->used++;
}

void __buf_set(struct buffer *buf, size_t at, void *ptr, size_t size)
{
	gr_null_check(ptr);
	buf_check(buf);

	if (size != buf->member_size) {
		fputs(WRONG_SIZE, stderr);
		return;
	}
	if (buf->used >= at) {
		fputs(OUT_OF_BOUNDS, stderr);
		return;
	}
	memcpy(buf_at(buf, at), ptr, buf->member_size);
}

void buf_trim(struct buffer *buf)
{
	buf_check(buf);

	buf->buf = realloc(buf->buf, buf->used * buf->member_size);
	buf->capacity = buf->used;
}

void buf_remove_range(struct buffer *buf, size_t from, size_t to)
{
	void *mov_to_addr;
	void *mov_from_addr;
	size_t mem_count;
	size_t byte_count;

	buf_check(buf);

	mov_to_addr = buf_at(buf, from);
	mov_from_addr = buf_at(buf, to + 1);
	byte_count = BYTE_COUNT(buf, to + 1, buf->used - 1);
	memmove(mov_to_addr, mov_from_addr, byte_count);
}

void buf_clear(struct buffer *buf)
{
	buf_check(buf);

	memset(buf->buf, 0, buf->capacity);
}

void buf_append_arr(struct buffer *buf, const void *arr, const size_t members)
{
	size_t i;
	void *endptr;
	size_t new_bytes;

	gr_null_check(arr);
	buf_check(buf);

	new_bytes = buf->member_size * members;
	if (!new_bytes) return;
	buf_ensure_capacity(buf, buf->used + members);
	endptr = buf_end_ptr(buf);
	memcpy(endptr, arr, new_bytes);
	buf->used += members;
}

size_t buf_read_f(struct buffer *buf, FILE *file, size_t amount)
{
	gr_null_check(file);
	buf_check(buf);

	return buf_read(buf, file, amount, (read_fn)fread);
}

size_t buf_read(
		struct buffer *buf,
		void *src,
		size_t amount,
		read_fn read)
{
	size_t ret;

	gr_null_check(src);
	buf_check(buf);

	buf_ensure_capacity(buf, buf->used + amount);
	ret = read(buf->buf + buf->used, buf->member_size, amount, src);
	buf->used += ret;

	return ret;
}

static void buf_ensure_capacity(struct buffer *buf, size_t capacity)
{
	buf_check(buf);

	while (capacity > buf->capacity) buf_grow(buf);
}

static void buf_grow(struct buffer *buf)
{
	buf_check(buf);

	buf->buf = realloc(buf->buf, buf->capacity * 2 * buf->member_size);
	buf->capacity *= 2;
}

static void * buf_end_ptr(const struct buffer * const buf)
{
	buf_check(buf);

	return buf->buf + buf->capacity * buf->member_size;
}
