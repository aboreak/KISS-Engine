#include "material.h"
#include "array.h"
#include "common.h"

#define LINE_LENGTH 64

struct material * material_new(const char *file_name)
{
	FILE *file = fopen(file_name, "r");
	if (!file) {
		fprintf(stderr, "%s doesn't exist\n", file_name);
		return NULL;
	}
	printf("Loading %s...\n", file_name);

	struct material *mtl = NEW(struct material);
	mtl->diffuse_map_file_name = NULL;

	char line[LINE_LENGTH];
	while (fgets(line, LINE_LENGTH, file)) {
		char *token;
		token = strtok(line, " ");
		if (strcmp(token, "Ns") == 0) {
			token = strtok(NULL, " ");
			mtl->shininess = atof(token);
		} else if (strcmp(token, "Ka") == 0) {
			token = strtok(NULL, " ");
			mtl->ambient.x = atof(token);
			token = strtok(NULL, " ");
			mtl->ambient.y = atof(token);
			token = strtok(NULL, " ");
			mtl->ambient.z = atof(token);
		} else if (strcmp(token, "Kd") == 0) {
			token = strtok(NULL, " ");
			mtl->diffuse.x = atof(token);
			token = strtok(NULL, " ");
			mtl->diffuse.y = atof(token);
			token = strtok(NULL, " ");
			mtl->diffuse.z = atof(token);
		} else if (strcmp(token, "Ks") == 0) {
			token = strtok(NULL, " ");
			mtl->specular.x = atof(token);
			token = strtok(NULL, " ");
			mtl->specular.y = atof(token);
			token = strtok(NULL, " ");
			mtl->specular.z = atof(token);
		} else if (strcmp(token, "map_Kd") == 0) {
			token = strtok(NULL, " \n\r");
			mtl->diffuse_map_file_name = malloc(strlen(token));
			strcpy(mtl->diffuse_map_file_name, token);
		}
	}

	fclose(file);

	return mtl;
}

void material_delete(struct material **mtl)
{
	free((*mtl)->diffuse_map_file_name);
	free(*mtl);
	*mtl = NULL;
}
