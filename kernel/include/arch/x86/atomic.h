#ifndef PhiOS_x86_atomic
#define PhiOS_x86_atomic

#include "include/types.h"
#include "include/compiler.h"

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

#endif
