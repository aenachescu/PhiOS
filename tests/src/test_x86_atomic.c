#include "tests/config.h"

// defines inline with nothing and after atomic.h is included then
// the inline keyword will disappear
// we need this hack because otherwise we have an error at linking,
// the reason being that the functions are not found... I think the
// compiler has a bug and it does not generate the code as it should,
// because if I enable the O2 flag then everything is fine...
#define inline
#include "kernel/include/arch/x86/atomic.h"

#include <pthread.h>

#define ATOMIC_UINT8_THREADS_NUM    (size_t) 4
#define ATOMIC_UINT8_ITERATIONS     (size_t) 1000001

atomic_uint8_t g_num8;

// test increment

void* atomic_uint8_inc_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT8_ITERATIONS; i++) {
        atomic_uint8_inc(&g_num8);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint8_inc)
{
    atomic_uint8_store(&g_num8, 0);

    pthread_t threads[ATOMIC_UINT8_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT8_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint8_inc_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT8_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_UINT8(atomic_uint8_load(&g_num8), ==, ATOMIC_UINT8_THREADS_NUM * ATOMIC_UINT8_ITERATIONS);
}

// test decrement

void* atomic_uint8_dec_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT8_ITERATIONS; i++) {
        atomic_uint8_dec(&g_num8);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint8_dec)
{
    atomic_uint8_store(&g_num8, 0);

    pthread_t threads[ATOMIC_UINT8_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT8_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint8_dec_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT8_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_UINT8(atomic_uint8_load(&g_num8), ==, 0 - ATOMIC_UINT8_THREADS_NUM * ATOMIC_UINT8_ITERATIONS);
}

// test add

void* atomic_uint8_add_thread_fn(void *param)
{
    uint8 value = (uint8) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT8_ITERATIONS; i++) {
        atomic_uint8_add(&g_num8, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint8_add)
{
    atomic_uint8_store(&g_num8, 0);

    pthread_t threads[ATOMIC_UINT8_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT8_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint8_add_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT8_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    uint8 expectedValue = 0;
    expectedValue += (ATOMIC_UINT8_ITERATIONS * 2);
    expectedValue += (ATOMIC_UINT8_ITERATIONS * 3);
    expectedValue += (ATOMIC_UINT8_ITERATIONS * 4);
    expectedValue += (ATOMIC_UINT8_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_UINT8(atomic_uint8_load(&g_num8), ==, expectedValue);
}

// test sub

void* atomic_uint8_sub_thread_fn(void *param)
{
    uint8 value = (uint8) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT8_ITERATIONS; i++) {
        atomic_uint8_sub(&g_num8, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint8_sub)
{
    atomic_uint8_store(&g_num8, 0);

    pthread_t threads[ATOMIC_UINT8_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT8_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint8_sub_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT8_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    uint8 expectedValue = 0;
    expectedValue -= (ATOMIC_UINT8_ITERATIONS * 2);
    expectedValue -= (ATOMIC_UINT8_ITERATIONS * 3);
    expectedValue -= (ATOMIC_UINT8_ITERATIONS * 4);
    expectedValue -= (ATOMIC_UINT8_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_UINT8(atomic_uint8_load(&g_num8), ==, expectedValue);
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_atomic_uint8_inc);
    CUT_CALL_TEST(test_atomic_uint8_dec);
    CUT_CALL_TEST(test_atomic_uint8_add);
    CUT_CALL_TEST(test_atomic_uint8_sub);
CUT_END_MAIN
