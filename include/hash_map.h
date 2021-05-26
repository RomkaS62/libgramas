#ifndef LIBGRAMAS_HASH_MAP_H
#define LIBGRAMAS_HASH_MAP_H

#include <stdint.h>
#include "ptr_array.h"
#include "gr_export.h"

GR_CDECL_BEGIN

struct GR_EXPORT gr_hash_map_entry {
	char *key;
	char *data;
	struct gr_hash_map_entry *prev;
	struct gr_hash_map_entry *next;
};

struct GR_EXPORT gr_hash_map {
	struct gr_hash_map_entry **entries;
	size_t bucket_capacity;
	size_t element_count;
	uint64_t (*hash)(char *key);
	int (*keys_equal)(char *key1, char *key2);
	void (*free_key)(char *key);	/* Frees hash table key upon deleting an entry */
	void (*free_data)(char *data);	/* Frees data associated with a key */
};

GR_EXPORT struct gr_hash_map *
gr_hash_map_new(
	uint64_t (*hash)(char *key),
	int (*keys_equal)(char *key1, char *key2),
	void (*free_key)(char *key),
	void (*free_data)(char *data));

GR_EXPORT void
gr_hash_map_init(
	struct gr_hash_map *map,
	uint64_t (*hash)(char *key), 
	int (*keys_equal)(char *key1, char *key2),
	void (*free_key)(char *key),
	void (*free_data)(char *data));

GR_EXPORT void gr_hash_map_delete(struct gr_hash_map *map);
GR_EXPORT void gr_hash_map_free(struct gr_hash_map *map);
GR_EXPORT void gr_hash_map_put(struct gr_hash_map *map, char *key, char *data);
GR_EXPORT void * gr_hash_map_get(struct gr_hash_map *map, char *key);
GR_EXPORT int gr_hash_map_remove(struct gr_hash_map *map, char *key);

GR_CDECL_END

#endif /* LIBGRAMAS_HASH_MAP_H */
