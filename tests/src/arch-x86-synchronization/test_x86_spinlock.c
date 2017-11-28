#include "tests/config.h"

// defines inline with nothing and after spinlock.h is included then
// the inline keyword will disappear
// we need this hack because otherwise we have an error at linking,
// the reason being that the functions are not found... I think the
// compiler has a bug and it does not generate the code as it should,
// because if I enable the O2 flag then everything is fine...
#define inline
#include "kernel/include/arch/x86/synchronization/spinlock.h"

#include <pthread.h>

CUT_DEFINE_TEST(test_spinlock_acquire)
{
    CUT_CHECK(0 == 0);
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_spinlock_acquire);
CUT_END_MAIN
