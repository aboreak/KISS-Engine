#ifndef __KISS_IO_H__
#define __KISS_IO_H__

#include "common.h"

static void read_file_to_memory(const char *file_name, void *buf)
{
	FILE *file;
	size_t file_size;
	void *buf;

	file = fopen(file_name, "r");
	if (!file) {
		error_errno(errno);
		return;
	}

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	fread(buf, 1, file_size, file);

	fclose(file);
}

#endif
