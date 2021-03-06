#ifndef __BITS_H__
#define __BITS_H__

#include <stdint.h>

static inline void rev32(uint32_t * x)
{
	*x = ((*x) & 0x55555555) << 1 | ((*x) & 0xAAAAAAAA) >> 1;
	*x = ((*x) & 0x33333333) << 2 | ((*x) & 0xCCCCCCCC) >> 2;
	*x = ((*x) & 0x0F0F0F0F) << 4 | ((*x) & 0xF0F0F0F0) >> 4;
	*x = ((*x) & 0x00FF00FF) << 8 | ((*x) & 0xFF00FF00) >> 8;
	*x = ((*x) & 0x0000FFFF) << 16 | ((*x) & 0xFFFF0000) >> 16;
}

static inline void rev16(uint16_t * x)
{
	*x = (*x & 0x5555) << 1 | (*x & 0xAAAA) >> 1;
	*x = (*x & 0x3333) << 2 | (*x & 0xCCCC) >> 2;
	*x = (*x & 0x0F0F) << 4 | (*x & 0xF0F0) >> 4;
	*x = (*x & 0x00FF) << 8 | (*x & 0xFF00) >> 8;
}

static inline void rev8(uint8_t * x)
{
	*x = (*x & 0x55) << 1 | (*x & 0xAA) >> 1;
	*x = (*x & 0x33) << 2 | (*x & 0xCC) >> 2;
	*x = (*x & 0x0F) << 4 | (*x & 0xF0) >> 4;
}

#endif
