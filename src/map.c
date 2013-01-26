#include "map.h"
#include "common.h"

struct map * map_new_real(unsigned key_size, unsigned value_size,
	unsigned max_len)
{
	struct map *map = NEW(struct map);

	map->key_size = key_size;
	map->value_size = value_size;
	map->keys = malloc(key_size * max_len);
	if (!map->keys) {
		fprintf(stderr, "map_new_real(): malloc() failed!");
		return NULL;
	}
	map->values = malloc(value_size * max_len);
	if (!map->values) {
		fprintf(stderr, "map_new_real(): malloc() failed!");
		return NULL;
	}
	map->max_len = max_len;
	map->len = 0;

	return map;
}

void map_delete(struct map **map)
{
	free((*map)->keys);
	free((*map)->values);
	free(*map);
	*map = NULL;
}

void map_add(struct map *map, void *key, void *value)
{
	unsigned char *free_key;
	unsigned char *free_value;

	if (map->len == map->max_len)
		return;

	free_key = map->keys + (map->key_size * map->len);
	free_value = map->values + (map->value_size * map->len);
	memcpy(free_key, key, map->key_size);
	memcpy(free_value, value, map->value_size);
	map->len++;
}

void map_remove(struct map *map, void *key)
{
	unsigned char *cur_key;
	unsigned char *cur_value;
	unsigned key_size;
	unsigned value_size;

	key_size = map->key_size;
	value_size = map->value_size;

	unsigned i;
	for (i = 0; i < map->len; i++) {
		cur_key = map->keys + (key_size * i);
		if (memcmp(cur_key, key, key_size) == 0)
			break;
	}

	for (; i < map->len; i++) {
		cur_key = map->keys + (key_size * i);
		cur_value = map->values + (value_size * i);
		memcpy(cur_key, cur_key + key_size, key_size);
		memcpy(cur_value, cur_value + value_size, value_size);
	}
	
	map->len--;
}

void * map_get(struct map *map, void *key)
{
	unsigned char *cur_key;
	unsigned key_size;

	key_size = map->key_size;

	unsigned i;
	for (i = 0; i < map->len; i++) {
		cur_key = map->keys + (key_size * i);
		if (memcmp(cur_key, key, key_size) == 0)
			return map->values + (i * map->value_size);
	}

	return NULL;
}
