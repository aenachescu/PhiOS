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

CUT_DEFINE_TEST(test_spinlock_init)
{
    spinlock_t spin;

    spinlock_init(&spin);
    CUT_CHECK_OPERATOR_UINT16(spin, ==, 0);
}

CUT_DEFINE_TEST(test_spinlock_try_lock)
{
    spinlock_t spin;
    spinlock_init(&spin);

    for (size_t i = 0; i < 1000; i++) {
        CUT_CHECK_OPERATOR_UINT8(spinlock_try_lock(&spin), ==, true);
        CUT_CHECK_OPERATOR_UINT16(spin, ==, 1);
        CUT_CHECK_OPERATOR_UINT8(spinlock_try_lock(&spin), ==, false);
        CUT_CHECK_OPERATOR_UINT16(spin, ==, 1);
        spinlock_release(&spin);
    }
}

CUT_DEFINE_TEST(test_spinlock_acquire)
{
    spinlock_t spin;
    spinlock_init(&spin);

    for (size_t i = 0; i < 1000; i++) {
        spinlock_acquire(&spin);
        CUT_CHECK_OPERATOR_UINT16(spin, ==, 1);
        CUT_CHECK_OPERATOR_UINT8(spinlock_try_lock(&spin), ==, false);
        CUT_CHECK_OPERATOR_UINT16(spin, ==, 1);
        spinlock_release(&spin);
    }
}

CUT_DEFINE_TEST(test_spinlock_release)
{
    spinlock_t spin;
    spinlock_init(&spin);

    for (size_t i = 0; i < 1000; i++) {
        spinlock_acquire(&spin);
        spinlock_release(&spin);
        CUT_CHECK_OPERATOR_UINT16(spin, ==, 0);
    }
}

#define THREADS_NUM (size_t) 4
#define ITERATIONS  (size_t) 1000001
#define RUNS        (size_t) 10

uint64 g_num = 0;
uint64 g_trylockNum[RUNS][THREADS_NUM];
size_t g_currentRun;
spinlock_t g_spin;

void* thread_fn(void *a_id)
{
    size_t id = (size_t) a_id;

    for (size_t i = 0; i < ITERATIONS; i++) {
        int type = rand() % 2;
        if (type == 0) {
            spinlock_acquire(&g_spin);
            g_num++;
            spinlock_release(&g_spin);
        } else {
            if (spinlock_try_lock(&g_spin)) {
                g_num++;
                spinlock_release(&g_spin);
            } else {
                g_trylockNum[g_currentRun][id]++;
            }
        }
    }

    return NULL;
}

CUT_DEFINE_TEST(test_spinlock)
{
    spinlock_init(&g_spin);
    srand(100);

    pthread_t threads[THREADS_NUM];
    int err;

    for (g_currentRun = 0; g_currentRun < RUNS; g_currentRun++) {
        g_num = 0;
        for (size_t i = 0; i < THREADS_NUM; i++) {
            g_trylockNum[g_currentRun][i] = 0;
            err = pthread_create(&threads[i], NULL, thread_fn, (void*) i);
            CUT_ASSERT(err == 0);
        }

        for (size_t i = 0; i < THREADS_NUM; i++) {
            err = pthread_join(threads[i], NULL);
            CUT_ASSERT(err == 0);
        }

        uint64 sum = g_num;
        for (size_t i = 0; i < THREADS_NUM; i++) {
            sum += g_trylockNum[g_currentRun][i];
        }

        CUT_CHECK_OPERATOR_UINT64(sum, ==, THREADS_NUM * ITERATIONS);
    }

    size_t equalValues;

    for (size_t i = 0; i < THREADS_NUM; i++) {
        equalValues = 0;
        for (size_t j = 0; j < RUNS - 1; j++) {
            for (size_t k = j + 1; k < RUNS; k++) {
                if (g_trylockNum[i][j] == g_trylockNum[i][k]) {
                    equalValues++;
                }
            }
        }

        CUT_CHECK_OPERATOR_SIZE_T(equalValues, <=, RUNS / 2);
    }

    for (size_t i = 0; i < RUNS; i++) {
        equalValues = 0;
        for (size_t j = 0; j < THREADS_NUM - 1; j++) {
            for (size_t k = j + 1; k < THREADS_NUM; k++) {
                if (g_trylockNum[i][j] == g_trylockNum[i][k]) {
                    equalValues++;
                }
            }
        }

        CUT_CHECK_OPERATOR_SIZE_T(equalValues, <=, THREADS_NUM / 2);
    }
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_spinlock_init);
    CUT_CALL_TEST(test_spinlock_try_lock);
    CUT_CALL_TEST(test_spinlock_acquire);
    CUT_CALL_TEST(test_spinlock_release);
    CUT_CALL_TEST(test_spinlock);
CUT_END_MAIN
