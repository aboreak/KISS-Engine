#ifndef __PROGRAM_H__
#define __PROGRAM_H__

extern unsigned g_program;

void program_startup(const char *vshader_file_name,
		     const char *fshader_file_name);

void program_shutdown();

#endif
