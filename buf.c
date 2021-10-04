#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gramas/buf.h"

#define GROW_FACTOR	(2)
#define BYTE_COUNT(b, from, to) ((b)->member_size * ((to) - (from)))

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
}

void buf_delete(struct buffer *buf)
{
	free(buf->buf);
}

void buf_free(struct buffer *buf)
{
	buf_delete(buf);
	free(buf);
}

void buf_append(struct buffer *buf, void *ptr)
{
	buf_ensure_capacity(buf, buf->used + 1);
	memcpy(buf_at(buf, buf->used), ptr, buf->member_size);
	buf->used++;
}

void __buf_set(struct buffer *buf, size_t at, void *ptr, size_t size)
{
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
	buf->buf = realloc(buf->buf, buf->used * buf->member_size);
	buf->capacity = buf->used;
}

void buf_remove_range(struct buffer *buf, size_t from, size_t to)
{
	void *mov_to_addr;
	void *mov_from_addr;
	size_t mem_count;
	size_t byte_count;

	mov_to_addr = buf_at(buf, from);
	mov_from_addr = buf_at(buf, to + 1);
	byte_count = BYTE_COUNT(buf, to + 1, buf->used - 1);
	memmove(mov_to_addr, mov_from_addr, byte_count);
}

void buf_clear(struct buffer *buf)
{
	memset(buf->buf, 0, buf->capacity);
}

void buf_append_arr(struct buffer *buf, const void *arr, const size_t members)
{
	size_t i;
	void *endptr;
	size_t new_bytes;

	new_bytes = buf->member_size * members;
	if (!new_bytes) return;
	buf_ensure_capacity(buf, buf->used + members);
	endptr = buf_end_ptr(buf);
	memcpy(endptr, arr, new_bytes);
	buf->used += members;
}

static void buf_ensure_capacity(struct buffer *buf, size_t capacity)
{
	while (capacity > buf->capacity) buf_grow(buf);
}

static void buf_grow(struct buffer *buf)
{
	buf->buf = realloc(buf->buf, buf->capacity * 2 * buf->member_size);
	buf->capacity *= 2;
}

static void * buf_end_ptr(const struct buffer * const buf)
{
	return buf->buf + buf->capacity * buf->member_size;
}
