#ifndef PhiOS_x86_atomic
#define PhiOS_x86_atomic

#include "include/types.h"
#include "include/compiler.h"

// atomic uint8
typedef uint8 atomic_uint8_t;

inline void atomic_uint8_add(atomic_uint8_t *a, uint8 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_uint8_inc(atomic_uint8_t *a)
{
    uint8 b = 1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_uint8_sub(atomic_uint8_t *a, uint8 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"((uint8) -b));
}

inline void atomic_uint8_dec(atomic_uint8_t *a)
{
    sint8 b = -1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_uint8_store(atomic_uint8_t *a, uint8 b)
{
    *a = b;
}

inline uint8 atomic_uint8_load(atomic_uint8_t *a)
{
    return *a;
}

// atomic int8
typedef sint8 atomic_int8_t;

inline void atomic_int8_add(atomic_int8_t *a, sint8 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_int8_inc(atomic_int8_t *a)
{
    sint8 b = 1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_int8_sub(atomic_int8_t *a, sint8 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"((sint8) -b));
}

inline void atomic_int8_dec(atomic_int8_t *a)
{
    sint8 b = -1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_int8_store(atomic_int8_t *a, sint8 b)
{
    *a = b;
}

inline sint8 atomic_int8_load(atomic_int8_t *a)
{
    return *a;
}

// atomic uint16
typedef uint16 atomic_uint16_t;

inline void atomic_uint16_add(atomic_uint16_t *a, uint16 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_uint16_inc(atomic_uint16_t *a)
{
    uint16 b = 1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_uint16_sub(atomic_uint16_t *a, uint16 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"((uint16) -b));
}

inline void atomic_uint16_dec(atomic_uint16_t *a)
{
    sint16 b = -1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_uint16_store(atomic_uint16_t *a, uint16 b)
{
    *a = b;
}

inline uint16 atomic_uint16_load(atomic_uint16_t *a)
{
    return *a;
}

// atomic int16
typedef sint16 atomic_int16_t;

inline void atomic_int16_add(atomic_int16_t *a, sint16 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_int16_inc(atomic_int16_t *a)
{
    sint16 b = 1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_int16_sub(atomic_int16_t *a, sint16 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"((sint16) -b));
}

inline void atomic_int16_dec(atomic_int16_t *a)
{
    sint16 b = -1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_int16_store(atomic_int16_t *a, sint16 b)
{
    *a = b;
}

inline sint16 atomic_int16_load(atomic_int16_t *a)
{
    return *a;
}

// atomic uint32
typedef uint32 atomic_uint32_t;

inline void atomic_uint32_add(atomic_uint32_t *a, uint32 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_uint32_inc(atomic_uint32_t *a)
{
    uint32 b = 1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_uint32_sub(atomic_uint32_t *a, uint32 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"((uint32) -b));
}

inline void atomic_uint32_dec(atomic_uint32_t *a)
{
    sint32 b = -1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_uint32_store(atomic_uint32_t *a, uint32 b)
{
    *a = b;
}

inline uint32 atomic_uint32_load(atomic_uint32_t *a)
{
    return *a;
}

// atomic int32
typedef sint32 atomic_int32_t;

inline void atomic_int32_add(atomic_int32_t *a, sint32 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_int32_inc(atomic_int32_t *a)
{
    sint32 b = 1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_int32_sub(atomic_int32_t *a, sint32 b)
{
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"((sint32) -b));
}

inline void atomic_int32_dec(atomic_int32_t *a)
{
    sint32 b = -1;
    asm volatile("lock xadd %1, %0" : "=m"(*a) : "r"(b));
}

inline void atomic_int32_store(atomic_int32_t *a, sint32 b)
{
    *a = b;
}

inline sint32 atomic_int32_load(atomic_int32_t *a)
{
    return *a;
}

#endif
