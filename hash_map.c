#include <stdlib.h>
#include <string.h>
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
	uint64_t (*hash)(const char *key),
	struct gr_hash_map_entry *entry);

static void
delete_entry(
	struct gr_hash_map_entry *entry,
	void (*free_key)(void *),
	void (*free_data)(void *));

struct gr_hash_map *
gr_hash_map_new(
	uint64_t (*hash)(const char *key),
	int (*keys_equal)(const char *key1, const char *key2),
	void (*free_key)(void *key),
	void (*free_data)(void *data))
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
	void (*free_key)(void *),
	void (*free_data)(void *));

void
gr_hash_map_init(
	struct gr_hash_map *map,
	uint64_t (*hash)(const char *key),
	int (*keys_equal)(const char *key1, const char *key2),
	void (*free_key)(void *key),
	void (*free_data)(void *data))
{
	size_t i;

	map->hash = hash;
	map->keys_equal = keys_equal;
	map->free_key = free_key;
	map->free_data = free_data;
	map->bucket_capacity = 8;
	map->element_count = 0;
	map->entries = malloc(sizeof(map->entries[0]) * map->bucket_capacity);
	for (i = 0; i < map->bucket_capacity; i++) {
		map->entries[i] = NULL;
	}
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

uint64_t gr_hash_hash_str(const char *str)
{
	uint64_t ret = 0;
	union {
		uint64_t i;
		char buf[sizeof(uint64_t)];
	} val;

	while (*str) {
		memset(val.buf, 0, sizeof(val.buf));
		strncpy(val.buf, str, sizeof(val.buf));
		ret ^= val.i << 13 ^ val.i >> 9 ^ val.i << 23;
		str++;
	}

	return ret;
}

int gr_hash_streq(const char *str1, const char *str2)
{
	return strcmp(str1, str2) == 0;
}

uint64_t gr_hash_hash_int(const char *_i)
{
	uint64_t i = *((int*)_i);

	return i << 14 ^ i >> 7 ^ i << 3;
}

int gr_hash_inteq(const char *a, const char *b)
{
	int int_a = *(int*)a;
	int int_b = *(int*)b;

	return int_a == int_b;
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

	return ret;
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
	uint64_t (*hash)(const char *key),
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
	void (*free_key)(void *),
	void (*free_data)(void *))
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
