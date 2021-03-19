#include <stdlib.h>
#include <string.h>

#include "hash_map.h"

static uint64_t int_hash(char *key);
static int int_eq(char *key1, char *key2);
static void int_free_key(char *key);
static void str_free_data(char *data);
static int * malloc_int(int i);
static char * copy_str(const char *str);

static int keys[] = {1, 2, 3, 4};
static char *values[] = {"One", "Two", "Three", "Four"};

int main(int argc, char **argv)
{
	int ret = 0;
	struct gr_hash_map map;

	gr_hash_map_init(&map, int_hash, int_eq, int_free_key, str_free_data);

	for (int i = 0; i < 4; i++) {
		gr_hash_map_put(&map, (char *)malloc_int(keys[i]), copy_str(values[i]));
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

static uint64_t int_hash(char *key)
{
	return *((int*)key);
}

static int int_eq(char *_key1, char *_key2)
{
	int key1;
	int key2;

	key1 = *((int *)_key1);
	key2 = *((int *)_key2);

	return key1 == key2;
}

static void int_free_key(char *key)
{
	free(key);
}

static void str_free_data(char *data)
{
	free(data);
}

static int * malloc_int(int i)
{
	int *ret;

	ret = malloc(sizeof(*ret));
	*ret = i;

	return ret;
}

static char * copy_str(const char *str)
{
	char *ret = malloc(strlen(str));

	strcpy(ret, str);

	return ret;
}