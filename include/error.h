#ifndef __ERROR_H__
#define __ERROR_H__

#ifndef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

void err_exit(const char *format, ...) NORETURN;

void err_exit2(const char *format, ...) NORETURN;

void err_exit_en(int errnum, const char *format, ...) NORETURN;

void fatal(const char *format, ...) NORETURN;

void err_usage(const char *format, ...) NORETURN;

void err_cmdline(const char *format, ...) NORETURN;

#endif
