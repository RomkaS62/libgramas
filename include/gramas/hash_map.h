#ifndef LIBGRAMAS_HASH_MAP_H
#define LIBGRAMAS_HASH_MAP_H

#include <stdint.h>
#include "gramas/ptr_array.h"
#include "gramas/gr_export.h"

GR_CDECL_BEGIN

typedef uint64_t (*hash_fn)(const char *key);
typedef int (*keys_equal_fn)(const char *key1, const char *key2);
typedef void (*free_key_fn)(void *key);
typedef void (*free_data_fn)(void *data);

struct gr_hash_map_entry {
	char *key;
	char *data;
	struct gr_hash_map_entry *prev;
	struct gr_hash_map_entry *next;
};

struct GR_EXPORT gr_hash_map {
	struct gr_hash_map_entry **entries;
	size_t bucket_capacity;
	size_t element_count;
	hash_fn hash;
	keys_equal_fn keys_equal;
	free_key_fn free_key;
	free_data_fn free_data;
};

GR_EXPORT struct gr_hash_map *
gr_hash_map_new(hash_fn, keys_equal_fn, free_key_fn, free_data_fn);

GR_EXPORT void
gr_hash_map_init(struct gr_hash_map *map,
		hash_fn, keys_equal_fn, free_key_fn, free_data_fn);

GR_EXPORT void gr_hash_map_delete(struct gr_hash_map *map);
GR_EXPORT void gr_hash_map_free(struct gr_hash_map *map);
GR_EXPORT void gr_hash_map_put(struct gr_hash_map *map, char *key, char *data);
GR_EXPORT void * gr_hash_map_get(struct gr_hash_map *map, char *key);
GR_EXPORT int gr_hash_map_remove(struct gr_hash_map *map, char *key);

/* --- Functions for working with common types --- */

uint64_t gr_hash_hash_str(const char *str);
int gr_hash_streq(const char *str1, const char *str2);

uint64_t gr_hash_hash_int(const char *i);
int gr_hash_inteq(const char *a, const char *b);

GR_CDECL_END

#endif /* LIBGRAMAS_HASH_MAP_H */
