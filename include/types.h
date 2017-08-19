#ifndef PhiOS_types
#define PhiOS_types

#ifdef PhiOS_ARCH_x86_32
#define WORDSIZE 32
#define WORDSIZE_BYTES 4
#define PhiOS32
#endif

#ifdef PhiOS_ARCH_x86_64
#define WORDSIZE 64
#define WORDSIZE_BYTES 8
#define PhiOS64
#endif

typedef unsigned long long          uint64;
typedef unsigned int                uint32;
typedef unsigned short              uint16;
typedef unsigned char               uint8;

typedef signed   long long          sint64;
typedef signed   int                sint32;
typedef signed   short              sint16;
typedef signed   char               sint8;

typedef const unsigned long long    cuint64;
typedef const unsigned int          cuint32;
typedef const unsigned short        cuint16;
typedef const unsigned char         cuint8;

typedef const signed   long long    csint64;
typedef const signed   int          csint32;
typedef const signed   short        csint16;
typedef const signed   char         csint8;

typedef char                        bool;

#define true  1
#define false 0

#if defined(PhiOS32) && !defined(UNIT_TEST)
#define NULL  0x00000000
typedef uint32 size_t;
#endif

#if defined(PhiOS64) && !defined(UNIT_TEST)
#define NULL  0x0000000000000000
typedef uint64 size_t;
#endif

#ifdef UNIT_TEST
#include <stddef.h>
#endif

#endif


