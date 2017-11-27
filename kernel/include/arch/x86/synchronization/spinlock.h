#ifndef PhiOS_x86_synchronization_spinlock
#define PhiOS_x86_synchronization_spinlock

#include "include/types.h"

typedef uint16 spinlock_t;

inline void spinlock_init(spinlock_t *spinlock)
{
    *spinlock = 0;
}

inline void spinlock_acquire(volatile spinlock_t *spinlock)
{
    asm volatile (
        // try to lock the spin
        ".acquireLock%=:\n"
            "lock bts %1, %0\n"
            "jc .spinWithPause%=\n"
            "jmp .acquireLockEnd%=\n"


        // spin with pause while the spin is locked
        ".spinWithPause%=:\n"
            "pause\n"
            "test %2, %0\n"
            "jnz .spinWithPause%=\n"
            "jmp .acquireLock%=\n" // if the spin is not locked then try to lock

        ".acquireLockEnd%=:\n"

        : "=m"(*spinlock)
        : "r"((uint16) 0), "r"((uint16) 1)
    );
}

inline void spinlock_release(spinlock_t *spinlock)
{
    *spinlock = 0;
}

inline bool spinlock_try_lock(spinlock_t *spinlock)
{
    volatile uint16 success = 1;
    asm volatile (
            "lock bts %2, %1\n"
            "jc .tryLockFailed%=\n"
            "jmp .tryLockSuccess%=\n"

        ".tryLockFailed%=:\n"
            "mov %2, %0\n"

        ".tryLockSuccess%=:\n"

        : "=m"(success), "=m"(*spinlock)
        : "r"((uint16) 0)
    );

    if (success == 1) {
        return true;
    }

    return false;
}

#endif
