#ifndef __MAP_H__
#define __MAP_H__

struct map {
	unsigned char *keys;
	unsigned char *values;
	unsigned key_size;
	unsigned value_size;
	unsigned max_len;
	unsigned len;
};

#define map_new(x,y,z) (map_new_real(sizeof(x), sizeof(y), z))

struct map *map_new_real(unsigned key_size, unsigned value_size,
			 unsigned max_len);

void map_delete(struct map **mp);

void map_add(struct map *mp, void *key, void *value);

void map_remove(struct map *mp, void *key);

void *map_get(struct map *mp, void *key);

#endif
