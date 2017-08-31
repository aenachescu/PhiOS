#ifndef PhiOS_UnitTesting_DataTypesAsserts
#define PhiOS_UnitTesting_DataTypesAsserts

#include "include/types.h"
#include <stdint.h>

#define static_assert _Static_assert
#define ASSERT_MESSAGE "incompatible data types"

static_assert(WORDSIZE_BYTES == sizeof(size_t), ASSERT_MESSAGE);

static_assert(sizeof(uint8) == sizeof(uint8_t), ASSERT_MESSAGE);
static_assert(sizeof(uint16) == sizeof(uint16_t), ASSERT_MESSAGE);
static_assert(sizeof(uint32) == sizeof(uint32_t), ASSERT_MESSAGE);
static_assert(sizeof(uint64) == sizeof(uint64_t), ASSERT_MESSAGE);

static_assert(sizeof(sint8) == sizeof(int8_t), ASSERT_MESSAGE);
static_assert(sizeof(sint16) == sizeof(int16_t), ASSERT_MESSAGE);
static_assert(sizeof(sint32) == sizeof(int32_t), ASSERT_MESSAGE);
static_assert(sizeof(sint64) == sizeof(int64_t), ASSERT_MESSAGE);

#undef static_assert
#undef ASSERT_MESSAGE

#endif
