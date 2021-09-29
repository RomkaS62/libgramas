#include <stdlib.h>
#include <string.h>

#include "hash_map.h"

static uint64_t int_hash(const char *key);
static int int_eq(const char *key1, const char *key2);
static int * malloc_int(int i);

static int keys[] = {1, 2, 3, 4};
static char *values[] = {"One", "Two", "Three", "Four"};

int main(int argc, char **argv)
{
	int ret = 0;
	struct gr_hash_map map;

	gr_hash_map_init(&map, int_hash, int_eq, free, free);

	for (int i = 0; i < 4; i++) {
		char *key = (char *)malloc_int(keys[i]);
		char *value = strdup(values[i]);
		gr_hash_map_put(&map, key, value);
	}

	for (int i = 0; i < 4; i++) {
		if (strcmp(gr_hash_map_get(&map, (char *)&keys[i]), values[i])) {
			ret = 1;
			goto fail;
		}
	}

fail:
	gr_hash_map_delete(&map);

	return ret;
}

static uint64_t int_hash(const char *key)
{
	return *((int*)key);
}

static int int_eq(const char *_key1, const char *_key2)
{
	int key1;
	int key2;

	key1 = *((int *)_key1);
	key2 = *((int *)_key2);

	return key1 == key2;
}

static int * malloc_int(int i)
{
	int *ret;

	ret = malloc(sizeof(*ret));
	*ret = i;

	return ret;
}
