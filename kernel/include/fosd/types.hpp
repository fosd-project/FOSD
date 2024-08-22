#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <architecture_limit.hpp>

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned int size_t;

#define UINT8_LIMIT  0xFF
#define UINT16_LIMIT 0xFFFF
#define UINT32_LIMIT 0xFFFFFFFF
#define UINT64_LIMIT 0xFFFFFFFFFFFFFFFF

#if WORD_SIZE == 64
    typedef uint64_t max_t;
    #define ARCH_LIMIT UINT64_LIMIT
#elif WORD_SIZE == 32
    typedef uint32_t max_t;
    #define ARCH_LIMIT UINT32_LIMIT
#elif WORD_SIZE == 16
    typedef uint16_t max_t;
    #define ARCH_LIMIT UINT16_LIMIT
#elif WORD_SIZE == 8
    typedef uint8_t max_t;
    #define ARCH_LIMIT UINT8_LIMIT
#endif

#define INVALID ARCH_LIMIT
// The indicator for functions that are architecture-dependent
#define ARCHDEP

/****** essential stuff ******/
#define MIN(X , Y) ((X) >= (Y) ? (Y) : (X))
#define MAX(X , Y) ((X) >= (Y) ? (X) : (Y))
#define ABS(X)     (((X) > 0) ? (X) : (-(X)))

#endif