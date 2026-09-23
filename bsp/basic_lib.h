#ifndef BASIC_LIB_H
#define BASIC_LIB_H

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IN_RANGE(value, min, max) (((value) >= (min)) && ((value) <= (max)))
#define IN_RANGE_EX(value, min, max) (((value) > (min)) && ((value) < (max)))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define BASIC_BIT(n)          (1UL << (n))
#define BASIC_SET_BIT(reg, n) ((reg) |= BASIC_BIT(n))
#define BASIC_CLR_BIT(reg, n) ((reg) &= ~BASIC_BIT(n))
#define BASIC_TGL_BIT(reg, n) ((reg) ^= BASIC_BIT(n))
#define BASIC_GET_BIT(reg, n) (((reg) >> (n)) & 1U)

#endif