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

typedef uint8                       bool;

#define true  (uint8) 1
#define false (uint8) 0

#if defined(PhiOS32) && !defined(PhiOS_UNIT_TESTING_CONFIG)
#define NULL  0x00000000
typedef uint32 size_t;
#endif

#if defined(PhiOS64) && !defined(PhiOS_UNIT_TESTING_CONFIG)
#define NULL  0x0000000000000000
typedef uint64 size_t;
#endif

#ifdef PhiOS_UNIT_TESTING_CONFIG
#include <stddef.h>
#endif

#define PhiOS_STATIC_ASSERT  _Static_assert
#define PhiOS_ASSERT_MESSAGE "Type size is not as expected."

PhiOS_STATIC_ASSERT(sizeof(uint64) == 8, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(uint32) == 4, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(uint16) == 2, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(uint8) == 1, PhiOS_ASSERT_MESSAGE);

PhiOS_STATIC_ASSERT(sizeof(sint64) == 8, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(sint32) == 4, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(sint16) == 2, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(sint8) == 1, PhiOS_ASSERT_MESSAGE);

PhiOS_STATIC_ASSERT(sizeof(cuint64) == 8, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(cuint32) == 4, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(cuint16) == 2, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(cuint8) == 1, PhiOS_ASSERT_MESSAGE);

PhiOS_STATIC_ASSERT(sizeof(csint64) == 8, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(csint32) == 4, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(csint16) == 2, PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(csint8) == 1, PhiOS_ASSERT_MESSAGE);

#ifdef PhiOS_UNIT_TESTING_CONFIG
#define PhiOS_STATIC
#else
#define PhiOS_STATIC static
#endif

#endif
