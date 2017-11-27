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

// test atomic_uint8_t

#define ATOMIC_UINT8_THREADS_NUM    (size_t) 4
#define ATOMIC_UINT8_ITERATIONS     (size_t) 1000001

atomic_uint8_t g_uint8;

// test increment

void* atomic_uint8_inc_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT8_ITERATIONS; i++) {
        atomic_uint8_inc(&g_uint8);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint8_inc)
{
    atomic_uint8_store(&g_uint8, 0);

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

    CUT_CHECK_OPERATOR_UINT8(atomic_uint8_load(&g_uint8), ==, ATOMIC_UINT8_THREADS_NUM * ATOMIC_UINT8_ITERATIONS);
}

// test decrement

void* atomic_uint8_dec_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT8_ITERATIONS; i++) {
        atomic_uint8_dec(&g_uint8);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint8_dec)
{
    atomic_uint8_store(&g_uint8, 0);

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

    CUT_CHECK_OPERATOR_UINT8(atomic_uint8_load(&g_uint8), ==, 0 - ATOMIC_UINT8_THREADS_NUM * ATOMIC_UINT8_ITERATIONS);
}

// test add

void* atomic_uint8_add_thread_fn(void *param)
{
    uint8 value = (uint8) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT8_ITERATIONS; i++) {
        atomic_uint8_add(&g_uint8, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint8_add)
{
    atomic_uint8_store(&g_uint8, 0);

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

    CUT_CHECK_OPERATOR_UINT8(atomic_uint8_load(&g_uint8), ==, expectedValue);
}

// test sub

void* atomic_uint8_sub_thread_fn(void *param)
{
    uint8 value = (uint8) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT8_ITERATIONS; i++) {
        atomic_uint8_sub(&g_uint8, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint8_sub)
{
    atomic_uint8_store(&g_uint8, 0);

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

    CUT_CHECK_OPERATOR_UINT8(atomic_uint8_load(&g_uint8), ==, expectedValue);
}

// test atomic_int8_t

#define ATOMIC_INT8_THREADS_NUM    (size_t) 4
#define ATOMIC_INT8_ITERATIONS     (size_t) 1000001

atomic_int8_t g_int8;

// test increment

void* atomic_int8_inc_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_INT8_ITERATIONS; i++) {
        atomic_int8_inc(&g_int8);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int8_inc)
{
    atomic_int8_store(&g_int8, 0);

    pthread_t threads[ATOMIC_INT8_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT8_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int8_inc_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT8_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_INT8(atomic_int8_load(&g_int8), ==, ATOMIC_INT8_THREADS_NUM * ATOMIC_INT8_ITERATIONS);
}

// test decrement

void* atomic_int8_dec_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_INT8_ITERATIONS; i++) {
        atomic_int8_dec(&g_int8);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int8_dec)
{
    atomic_int8_store(&g_int8, 0);

    pthread_t threads[ATOMIC_INT8_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT8_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int8_dec_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT8_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_INT8(atomic_int8_load(&g_int8), ==, 0 - ATOMIC_INT8_THREADS_NUM * ATOMIC_INT8_ITERATIONS);
}

// test add

void* atomic_int8_add_thread_fn(void *param)
{
    sint8 value = (sint8) (size_t) param;
    for (size_t i = 0; i < ATOMIC_INT8_ITERATIONS; i++) {
        atomic_int8_add(&g_int8, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int8_add)
{
    atomic_int8_store(&g_int8, 0);

    pthread_t threads[ATOMIC_INT8_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT8_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int8_add_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT8_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    sint8 expectedValue = 0;
    expectedValue += (ATOMIC_INT8_ITERATIONS * 2);
    expectedValue += (ATOMIC_INT8_ITERATIONS * 3);
    expectedValue += (ATOMIC_INT8_ITERATIONS * 4);
    expectedValue += (ATOMIC_INT8_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_INT8(atomic_int8_load(&g_int8), ==, expectedValue);
}

// test sub

void* atomic_int8_sub_thread_fn(void *param)
{
    sint8 value = (sint8) (size_t) param;
    for (size_t i = 0; i < ATOMIC_INT8_ITERATIONS; i++) {
        atomic_int8_sub(&g_int8, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int8_sub)
{
    atomic_int8_store(&g_int8, 0);

    pthread_t threads[ATOMIC_INT8_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT8_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int8_sub_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT8_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    sint8 expectedValue = 0;
    expectedValue -= (ATOMIC_INT8_ITERATIONS * 2);
    expectedValue -= (ATOMIC_INT8_ITERATIONS * 3);
    expectedValue -= (ATOMIC_INT8_ITERATIONS * 4);
    expectedValue -= (ATOMIC_INT8_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_INT8(atomic_int8_load(&g_int8), ==, expectedValue);
}

// test atomic_uint16_t

#define ATOMIC_UINT16_THREADS_NUM    (size_t) 4
#define ATOMIC_UINT16_ITERATIONS     (size_t) 1000001

atomic_uint16_t g_uint16;

// test increment

void* atomic_uint16_inc_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT16_ITERATIONS; i++) {
        atomic_uint16_inc(&g_uint16);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint16_inc)
{
    atomic_uint16_store(&g_uint16, 0);

    pthread_t threads[ATOMIC_UINT16_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT16_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint16_inc_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT16_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_UINT16(atomic_uint16_load(&g_uint16), ==, ATOMIC_UINT16_THREADS_NUM * ATOMIC_UINT16_ITERATIONS);
}

// test decrement

void* atomic_uint16_dec_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT16_ITERATIONS; i++) {
        atomic_uint16_dec(&g_uint16);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint16_dec)
{
    atomic_uint16_store(&g_uint16, 0);

    pthread_t threads[ATOMIC_UINT16_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT16_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint16_dec_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT16_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_UINT16(atomic_uint16_load(&g_uint16), ==, 0 - ATOMIC_UINT16_THREADS_NUM * ATOMIC_UINT16_ITERATIONS);
}

// test add

void* atomic_uint16_add_thread_fn(void *param)
{
    uint16 value = (uint16) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT16_ITERATIONS; i++) {
        atomic_uint16_add(&g_uint16, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint16_add)
{
    atomic_uint16_store(&g_uint16, 0);

    pthread_t threads[ATOMIC_UINT16_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT16_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint16_add_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT16_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    uint16 expectedValue = 0;
    expectedValue += (ATOMIC_UINT16_ITERATIONS * 2);
    expectedValue += (ATOMIC_UINT16_ITERATIONS * 3);
    expectedValue += (ATOMIC_UINT16_ITERATIONS * 4);
    expectedValue += (ATOMIC_UINT16_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_UINT16(atomic_uint16_load(&g_uint16), ==, expectedValue);
}

// test sub

void* atomic_uint16_sub_thread_fn(void *param)
{
    uint16 value = (uint16) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT16_ITERATIONS; i++) {
        atomic_uint16_sub(&g_uint16, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint16_sub)
{
    atomic_uint16_store(&g_uint16, 0);

    pthread_t threads[ATOMIC_UINT16_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT16_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint16_sub_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT16_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    uint16 expectedValue = 0;
    expectedValue -= (ATOMIC_UINT16_ITERATIONS * 2);
    expectedValue -= (ATOMIC_UINT16_ITERATIONS * 3);
    expectedValue -= (ATOMIC_UINT16_ITERATIONS * 4);
    expectedValue -= (ATOMIC_UINT16_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_UINT16(atomic_uint16_load(&g_uint16), ==, expectedValue);
}

// test atomic_int16_t

#define ATOMIC_INT16_THREADS_NUM    (size_t) 4
#define ATOMIC_INT16_ITERATIONS     (size_t) 1000001

atomic_int16_t g_int16;

// test increment

void* atomic_int16_inc_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_INT16_ITERATIONS; i++) {
        atomic_int16_inc(&g_int16);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int16_inc)
{
    atomic_int16_store(&g_int16, 0);

    pthread_t threads[ATOMIC_INT16_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT16_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int16_inc_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT16_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_INT16(atomic_int16_load(&g_int16), ==, ATOMIC_INT16_THREADS_NUM * ATOMIC_INT16_ITERATIONS);
}

// test decrement

void* atomic_int16_dec_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_INT16_ITERATIONS; i++) {
        atomic_int16_dec(&g_int16);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int16_dec)
{
    atomic_int16_store(&g_int16, 0);

    pthread_t threads[ATOMIC_INT16_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT16_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int16_dec_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT16_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_INT16(atomic_int16_load(&g_int16), ==, 0 - ATOMIC_INT16_THREADS_NUM * ATOMIC_INT16_ITERATIONS);
}

// test add

void* atomic_int16_add_thread_fn(void *param)
{
    sint16 value =(sint16) (size_t) param;
    for (size_t i = 0; i < ATOMIC_INT16_ITERATIONS; i++) {
        atomic_int16_add(&g_int16, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int16_add)
{
    atomic_int16_store(&g_int16, 0);

    pthread_t threads[ATOMIC_INT16_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT16_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int16_add_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT16_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    sint16 expectedValue = 0;
    expectedValue += (ATOMIC_INT16_ITERATIONS * 2);
    expectedValue += (ATOMIC_INT16_ITERATIONS * 3);
    expectedValue += (ATOMIC_INT16_ITERATIONS * 4);
    expectedValue += (ATOMIC_INT16_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_INT16(atomic_int16_load(&g_int16), ==, expectedValue);
}

// test sub

void* atomic_int16_sub_thread_fn(void *param)
{
    sint16 value =(sint16) (size_t) param;
    for (size_t i = 0; i < ATOMIC_INT16_ITERATIONS; i++) {
        atomic_int16_sub(&g_int16, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int16_sub)
{
    atomic_int16_store(&g_int16, 0);

    pthread_t threads[ATOMIC_INT16_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT16_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int16_sub_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT16_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    sint16 expectedValue = 0;
    expectedValue -= (ATOMIC_INT16_ITERATIONS * 2);
    expectedValue -= (ATOMIC_INT16_ITERATIONS * 3);
    expectedValue -= (ATOMIC_INT16_ITERATIONS * 4);
    expectedValue -= (ATOMIC_INT16_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_INT16(atomic_int16_load(&g_int16), ==, expectedValue);
}

// test atomic_uint32_t

#define ATOMIC_UINT32_THREADS_NUM    (size_t) 4
#define ATOMIC_UINT32_ITERATIONS     (size_t) 1000001

atomic_uint32_t g_uint32;

// test increment

void* atomic_uint32_inc_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT32_ITERATIONS; i++) {
        atomic_uint32_inc(&g_uint32);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint32_inc)
{
    atomic_uint32_store(&g_uint32, 0);

    pthread_t threads[ATOMIC_UINT32_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT32_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint32_inc_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT32_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_UINT32(atomic_uint32_load(&g_uint32), ==, ATOMIC_UINT32_THREADS_NUM * ATOMIC_UINT32_ITERATIONS);
}

// test decrement

void* atomic_uint32_dec_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT32_ITERATIONS; i++) {
        atomic_uint32_dec(&g_uint32);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint32_dec)
{
    atomic_uint32_store(&g_uint32, 0);

    pthread_t threads[ATOMIC_UINT32_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT32_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint32_dec_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT32_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_UINT32(atomic_uint32_load(&g_uint32), ==, 0 - ATOMIC_UINT32_THREADS_NUM * ATOMIC_UINT32_ITERATIONS);
}

// test add

void* atomic_uint32_add_thread_fn(void *param)
{
    uint32 value = (uint32) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT32_ITERATIONS; i++) {
        atomic_uint32_add(&g_uint32, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint32_add)
{
    atomic_uint32_store(&g_uint32, 0);

    pthread_t threads[ATOMIC_UINT32_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT32_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint32_add_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT32_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    uint32 expectedValue = 0;
    expectedValue += (ATOMIC_UINT32_ITERATIONS * 2);
    expectedValue += (ATOMIC_UINT32_ITERATIONS * 3);
    expectedValue += (ATOMIC_UINT32_ITERATIONS * 4);
    expectedValue += (ATOMIC_UINT32_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_UINT32(atomic_uint32_load(&g_uint32), ==, expectedValue);
}

// test sub

void* atomic_uint32_sub_thread_fn(void *param)
{
    uint32 value = (uint32) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT32_ITERATIONS; i++) {
        atomic_uint32_sub(&g_uint32, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint32_sub)
{
    atomic_uint32_store(&g_uint32, 0);

    pthread_t threads[ATOMIC_UINT32_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT32_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint32_sub_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT32_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    uint32 expectedValue = 0;
    expectedValue -= (ATOMIC_UINT32_ITERATIONS * 2);
    expectedValue -= (ATOMIC_UINT32_ITERATIONS * 3);
    expectedValue -= (ATOMIC_UINT32_ITERATIONS * 4);
    expectedValue -= (ATOMIC_UINT32_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_UINT32(atomic_uint32_load(&g_uint32), ==, expectedValue);
}

// test atomic_int32_t

#define ATOMIC_INT32_THREADS_NUM    (size_t) 4
#define ATOMIC_INT32_ITERATIONS     (size_t) 1000001

atomic_int32_t g_int32;

// test increment

void* atomic_int32_inc_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_INT32_ITERATIONS; i++) {
        atomic_int32_inc(&g_int32);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int32_inc)
{
    atomic_int32_store(&g_int32, 0);

    pthread_t threads[ATOMIC_INT32_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT32_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int32_inc_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT32_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_INT32(atomic_int32_load(&g_int32), ==, ATOMIC_INT32_THREADS_NUM * ATOMIC_INT32_ITERATIONS);
}

// test decrement

void* atomic_int32_dec_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_INT32_ITERATIONS; i++) {
        atomic_int32_dec(&g_int32);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int32_dec)
{
    atomic_int32_store(&g_int32, 0);

    pthread_t threads[ATOMIC_INT32_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT32_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int32_dec_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT32_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_INT32(atomic_int32_load(&g_int32), ==, 0 - ATOMIC_INT32_THREADS_NUM * ATOMIC_INT32_ITERATIONS);
}

// test add

void* atomic_int32_add_thread_fn(void *param)
{
    sint32 value = (sint32) (size_t) param;
    for (size_t i = 0; i < ATOMIC_INT32_ITERATIONS; i++) {
        atomic_int32_add(&g_int32, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int32_add)
{
    atomic_int32_store(&g_int32, 0);

    pthread_t threads[ATOMIC_INT32_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT32_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int32_add_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT32_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    sint32 expectedValue = 0;
    expectedValue += (ATOMIC_INT32_ITERATIONS * 2);
    expectedValue += (ATOMIC_INT32_ITERATIONS * 3);
    expectedValue += (ATOMIC_INT32_ITERATIONS * 4);
    expectedValue += (ATOMIC_INT32_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_INT32(atomic_int32_load(&g_int32), ==, expectedValue);
}

// test sub

void* atomic_int32_sub_thread_fn(void *param)
{
    sint32 value = (sint32) (size_t) param;
    for (size_t i = 0; i < ATOMIC_INT32_ITERATIONS; i++) {
        atomic_int32_sub(&g_int32, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int32_sub)
{
    atomic_int32_store(&g_int32, 0);

    pthread_t threads[ATOMIC_INT32_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT32_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int32_sub_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT32_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    sint32 expectedValue = 0;
    expectedValue -= (ATOMIC_INT32_ITERATIONS * 2);
    expectedValue -= (ATOMIC_INT32_ITERATIONS * 3);
    expectedValue -= (ATOMIC_INT32_ITERATIONS * 4);
    expectedValue -= (ATOMIC_INT32_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_INT32(atomic_int32_load(&g_int32), ==, expectedValue);
}

// test atomic_uint64_t

#define ATOMIC_UINT64_THREADS_NUM    (size_t) 4
#define ATOMIC_UINT64_ITERATIONS     (size_t) 1000001

atomic_uint64_t g_uint64;

// test increment

void* atomic_uint64_inc_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT64_ITERATIONS; i++) {
        atomic_uint64_inc(&g_uint64);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint64_inc)
{
    atomic_uint64_store(&g_uint64, 0);

    pthread_t threads[ATOMIC_UINT64_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT64_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint64_inc_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT64_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_UINT64(atomic_uint64_load(&g_uint64), ==, ATOMIC_UINT64_THREADS_NUM * ATOMIC_UINT64_ITERATIONS);
}

// test decrement

void* atomic_uint64_dec_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_UINT64_ITERATIONS; i++) {
        atomic_uint64_dec(&g_uint64);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint64_dec)
{
    atomic_uint64_store(&g_uint64, 0);

    pthread_t threads[ATOMIC_UINT64_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT64_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint64_dec_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT64_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_UINT64(atomic_uint64_load(&g_uint64), ==, ((uint64) 0) - ATOMIC_UINT64_THREADS_NUM * ATOMIC_UINT64_ITERATIONS);
}

// test add

void* atomic_uint64_add_thread_fn(void *param)
{
    uint64 value = (uint64) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT64_ITERATIONS; i++) {
        atomic_uint64_add(&g_uint64, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint64_add)
{
    atomic_uint64_store(&g_uint64, 0);

    pthread_t threads[ATOMIC_UINT64_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT64_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint64_add_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT64_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    uint64 expectedValue = 0;
    expectedValue += (ATOMIC_UINT64_ITERATIONS * 2);
    expectedValue += (ATOMIC_UINT64_ITERATIONS * 3);
    expectedValue += (ATOMIC_UINT64_ITERATIONS * 4);
    expectedValue += (ATOMIC_UINT64_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_UINT64(atomic_uint64_load(&g_uint64), ==, expectedValue);
}

// test sub

void* atomic_uint64_sub_thread_fn(void *param)
{
    uint64 value = (uint64) (size_t) param;
    for (size_t i = 0; i < ATOMIC_UINT64_ITERATIONS; i++) {
        atomic_uint64_sub(&g_uint64, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_uint64_sub)
{
    atomic_uint64_store(&g_uint64, 0);

    pthread_t threads[ATOMIC_UINT64_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_UINT64_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_uint64_sub_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_UINT64_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    uint64 expectedValue = 0;
    expectedValue -= (ATOMIC_UINT64_ITERATIONS * 2);
    expectedValue -= (ATOMIC_UINT64_ITERATIONS * 3);
    expectedValue -= (ATOMIC_UINT64_ITERATIONS * 4);
    expectedValue -= (ATOMIC_UINT64_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_UINT64(atomic_uint64_load(&g_uint64), ==, expectedValue);
}

// test atomic_int64_t

#define ATOMIC_INT64_THREADS_NUM    (size_t) 4
#define ATOMIC_INT64_ITERATIONS     (size_t) 1000001

atomic_int64_t g_int64;

// test increment

void* atomic_int64_inc_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_INT64_ITERATIONS; i++) {
        atomic_int64_inc(&g_int64);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int64_inc)
{
    atomic_int64_store(&g_int64, 0);

    pthread_t threads[ATOMIC_INT64_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT64_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int64_inc_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT64_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_INT64(atomic_int64_load(&g_int64), ==, ATOMIC_INT64_THREADS_NUM * ATOMIC_INT64_ITERATIONS);
}

// test decrement

void* atomic_int64_dec_thread_fn(void *unused)
{
    (void) unused;

    for (size_t i = 0; i < ATOMIC_INT64_ITERATIONS; i++) {
        atomic_int64_dec(&g_int64);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int64_dec)
{
    atomic_int64_store(&g_int64, 0);

    pthread_t threads[ATOMIC_INT64_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT64_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int64_dec_thread_fn, NULL);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT64_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    CUT_CHECK_OPERATOR_INT64(atomic_int64_load(&g_int64), ==, ((sint64) 0) - ATOMIC_INT64_THREADS_NUM * ATOMIC_INT64_ITERATIONS);
}

// test add

void* atomic_int64_add_thread_fn(void *param)
{
    sint64 value = (sint64) (size_t) param;
    for (size_t i = 0; i < ATOMIC_INT64_ITERATIONS; i++) {
        atomic_int64_add(&g_int64, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int64_add)
{
    atomic_int64_store(&g_int64, 0);

    pthread_t threads[ATOMIC_INT64_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT64_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int64_add_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT64_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    sint64 expectedValue = 0;
    expectedValue += (ATOMIC_INT64_ITERATIONS * 2);
    expectedValue += (ATOMIC_INT64_ITERATIONS * 3);
    expectedValue += (ATOMIC_INT64_ITERATIONS * 4);
    expectedValue += (ATOMIC_INT64_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_INT64(atomic_int64_load(&g_int64), ==, expectedValue);
}

// test sub

void* atomic_int64_sub_thread_fn(void *param)
{
    sint64 value = (sint64) (size_t) param;
    for (size_t i = 0; i < ATOMIC_INT64_ITERATIONS; i++) {
        atomic_int64_sub(&g_int64, value);
    }

    return NULL;
}

CUT_DEFINE_TEST(test_atomic_int64_sub)
{
    atomic_int64_store(&g_int64, 0);

    pthread_t threads[ATOMIC_INT64_THREADS_NUM];
    int err;

    for (size_t i = 0; i < ATOMIC_INT64_THREADS_NUM; i++) {
        err = pthread_create(&threads[i], NULL, atomic_int64_sub_thread_fn, (void*) i + 2);
        CUT_ASSERT(err == 0);
    }

    for (size_t i = 0; i < ATOMIC_INT64_THREADS_NUM; i++) {
        err = pthread_join(threads[i], NULL);
        CUT_ASSERT(err == 0);
    }

    sint64 expectedValue = 0;
    expectedValue -= (ATOMIC_INT64_ITERATIONS * 2);
    expectedValue -= (ATOMIC_INT64_ITERATIONS * 3);
    expectedValue -= (ATOMIC_INT64_ITERATIONS * 4);
    expectedValue -= (ATOMIC_INT64_ITERATIONS * 5);

    CUT_CHECK_OPERATOR_INT64(atomic_int64_load(&g_int64), ==, expectedValue);
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_atomic_uint8_inc);
    CUT_CALL_TEST(test_atomic_uint8_dec);
    CUT_CALL_TEST(test_atomic_uint8_add);
    CUT_CALL_TEST(test_atomic_uint8_sub);

    CUT_CALL_TEST(test_atomic_int8_inc);
    CUT_CALL_TEST(test_atomic_int8_dec);
    CUT_CALL_TEST(test_atomic_int8_add);
    CUT_CALL_TEST(test_atomic_int8_sub);

    CUT_CALL_TEST(test_atomic_uint16_inc);
    CUT_CALL_TEST(test_atomic_uint16_dec);
    CUT_CALL_TEST(test_atomic_uint16_add);
    CUT_CALL_TEST(test_atomic_uint16_sub);

    CUT_CALL_TEST(test_atomic_int16_inc);
    CUT_CALL_TEST(test_atomic_int16_dec);
    CUT_CALL_TEST(test_atomic_int16_add);
    CUT_CALL_TEST(test_atomic_int16_sub);

    CUT_CALL_TEST(test_atomic_uint32_inc);
    CUT_CALL_TEST(test_atomic_uint32_dec);
    CUT_CALL_TEST(test_atomic_uint32_add);
    CUT_CALL_TEST(test_atomic_uint32_sub);

    CUT_CALL_TEST(test_atomic_int32_inc);
    CUT_CALL_TEST(test_atomic_int32_dec);
    CUT_CALL_TEST(test_atomic_int32_add);
    CUT_CALL_TEST(test_atomic_int32_sub);

    CUT_CALL_TEST(test_atomic_uint64_inc);
    CUT_CALL_TEST(test_atomic_uint64_dec);
    CUT_CALL_TEST(test_atomic_uint64_add);
    CUT_CALL_TEST(test_atomic_uint64_sub);

    CUT_CALL_TEST(test_atomic_int64_inc);
    CUT_CALL_TEST(test_atomic_int64_dec);
    CUT_CALL_TEST(test_atomic_int64_add);
    CUT_CALL_TEST(test_atomic_int64_sub);

CUT_END_MAIN
