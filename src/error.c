#include "error.h"
#include "common.h"

inline void error_errno(int errnum)
{
	fprintf(stderr, "%s\n", strerror(errnum));
}

inline void error_msg(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
}
