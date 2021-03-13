#include <stdlib.h>
#include "hash_map.h"

static struct gr_hash_map_entry *
gr_hash_map_entry_new(
	char *key,
	char *data);

static void consider_rehash(struct gr_hash_map *map);
static void rehash(struct gr_hash_map *map, size_t new_size);

static void
insert_entry(
	struct gr_hash_map_entry **table,
	size_t capacity,
	uint64_t (*hash)(char *key),
	struct gr_hash_map_entry *entry);

static void
delete_entry(
	struct gr_hash_map_entry *entry,
	void (*free_key)(char *),
	void (*free_data)(char *));

struct gr_hash_map *
gr_hash_map_new(
	uint64_t (*hash)(char *key),
	int (*keys_equal)(char *key1, char *key2),
	void (*free_key)(char *key),
	void (*free_data)(char *data))
{
	struct gr_hash_map *ret = malloc(sizeof(*ret));

	gr_hash_map_init(ret, hash, keys_equal, free_key, free_data);
	return ret;
}

static void
remove_entry(
	struct gr_hash_map *map,
	size_t bucket_idx,
	struct gr_hash_map_entry *entry);

static void
free_entry(
	struct gr_hash_map_entry *entry,
	void (*free_key)(char *),
	void (*free_data)(char *));

void
gr_hash_map_init(
	struct gr_hash_map *map,
	uint64_t (*hash)(char *key),
	int (*keys_equal)(char *key1, char *key2),
	void (*free_key)(char *key),
	void (*free_data)(char *data))
{
	map->hash = hash;
	map->keys_equal = keys_equal;
	map->free_key = free_key;
	map->free_data = free_data;
	map->bucket_capacity = 8;
	map->element_count = 0;
	map->entries = malloc(sizeof(map->entries[0]) * map->bucket_capacity);
}

void gr_hash_map_delete(struct gr_hash_map *map)
{
	size_t i;
	struct gr_hash_map_entry *entry;
	struct gr_hash_map_entry *next;

	for (i = 0; i < map->bucket_capacity; i++) {
		entry = map->entries[i];
		if (entry != NULL) {
			next = entry->next;
			while (next != NULL) {
				free_entry(entry, map->free_key, map->free_data);
				entry = next;
				next = entry->next;
			}
			free_entry(entry, map->free_key, map->free_data);
		}
	}
	free(map->entries);
}

void gr_hash_map_free(struct gr_hash_map *map)
{
	gr_hash_map_delete(map);
	free(map);
}

void gr_hash_map_put(struct gr_hash_map *map, char *key, char *data)
{
	uint64_t hc;
	struct gr_hash_map_entry *entry;

	consider_rehash(map);
	hc = map->hash(key);
	entry = gr_hash_map_entry_new(key, data);
	insert_entry(map->entries, map->bucket_capacity, map->hash, entry);
	map->element_count++;
}

void * gr_hash_map_get(struct gr_hash_map *map, char *key)
{
	size_t bucket_idx;
	struct gr_hash_map_entry *entry;

	bucket_idx = map->hash(key) % map->bucket_capacity;
	entry = map->entries[bucket_idx];
	if (entry != NULL) {
		if (map->keys_equal(entry->key, key)) {
			return entry->data;
		} else {
			while ((entry = entry->next) != NULL) {
				if (map->keys_equal(entry->key, key)) {
					return entry->data;
				}
			}
		}
	}
	return NULL;
}

int gr_hash_map_remove(struct gr_hash_map *map, char *key)
{
	size_t bucket_idx;
	struct gr_hash_map_entry *entry;

	bucket_idx = map->hash(key) % map->bucket_capacity;
	entry = map->entries[bucket_idx];
	if (entry != NULL) {
		if (map->keys_equal(entry->key, key)) {
			remove_entry(map, bucket_idx, entry);
			return 1;
		} else {
			while ((entry = entry->next) != NULL) {
				if (map->keys_equal(entry->key, key)) {
					remove_entry(map, bucket_idx, entry);
					return 1;
				}
			}
		}
	}
	return 0;
}

static struct gr_hash_map_entry *
gr_hash_map_entry_new(
	char *key,
	char *data)
{
	struct gr_hash_map_entry *ret = malloc(sizeof(*ret));

	ret->key = key;
	ret->data = data;
	ret->prev = NULL;
	ret->next = NULL;
}

static void consider_rehash(struct gr_hash_map *map)
{
	if (map->element_count > map->bucket_capacity) {
		rehash(map, map->bucket_capacity * 2);
	}
}

static void rehash(struct gr_hash_map *map, size_t new_size)
{
	size_t i;
	struct gr_hash_map_entry ** new_table;
	struct gr_hash_map_entry *entry;

	new_table = malloc(sizeof(*new_table) * new_size);
	for (i = 0; i < map->bucket_capacity; i++) {
		if (map->entries[i] != NULL) {
			entry = map->entries[i];
			insert_entry(new_table, new_size, map->hash, entry);
			while ((entry = entry->next) != NULL) {
				insert_entry(new_table, new_size, map->hash, entry);
			}
		}
	}
}

static void
insert_entry(
	struct gr_hash_map_entry **table,
	size_t capacity,
	uint64_t (*hash)(char *key),
	struct gr_hash_map_entry *entry)
{
	uint64_t hc = hash(entry->key);
	size_t idx = hc % capacity;

	if (table[idx] == NULL) {
		table[idx] = entry;
	} else {
		table[idx]->prev = entry;
		entry->next = table[idx];
		table[idx] = entry;
	}
}

static void
free_entry(
	struct gr_hash_map_entry *entry,
	void (*free_key)(char *),
	void (*free_data)(char *))
{
	free_key(entry->key);
	free_data(entry->data);
	free(entry);
}

static void
remove_entry(
	struct gr_hash_map *map,
	size_t bucket_idx,
	struct gr_hash_map_entry *entry)
{
	if (map->entries[bucket_idx] == entry) {
		if (entry->next != NULL) entry->next->prev = NULL;
		map->entries[bucket_idx] = entry->next;
		free_entry(entry, map->free_key, map->free_data);
	} else if (entry->next == NULL) {
		entry->prev->next = NULL;
		free_entry(entry, map->free_key, map->free_data);
	} else {
		entry->prev->next = entry->next;
		entry->next->prev = entry->prev;
		free_entry(entry, map->free_key, map->free_data);
	}
	map->element_count--;
}
