// Our favourite types!

#pragma once

typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;

#include <stdbool.h>

#define ABS(a) (((a)<0) ? -(a) : (a))
#define MAX(a, b) (((a)<(b)) ? (b) : (a))
#define MIN(a, b) (((a)<(b)) ? (a) : (b))
	
#define PACKED __attribute__((packed))