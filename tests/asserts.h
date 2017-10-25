#ifndef PhiOS_UnitTesting_DataTypesAsserts
#define PhiOS_UnitTesting_DataTypesAsserts

#include "include/types.h"
#include <stdint.h>

PhiOS_STATIC_ASSERT(WORDSIZE_BYTES == sizeof(size_t), PhiOS_ASSERT_MESSAGE);

PhiOS_STATIC_ASSERT(sizeof(uint8) == sizeof(uint8_t), PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(uint16) == sizeof(uint16_t), PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(uint32) == sizeof(uint32_t), PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(uint64) == sizeof(uint64_t), PhiOS_ASSERT_MESSAGE);

PhiOS_STATIC_ASSERT(sizeof(sint8) == sizeof(int8_t), PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(sint16) == sizeof(int16_t), PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(sint32) == sizeof(int32_t), PhiOS_ASSERT_MESSAGE);
PhiOS_STATIC_ASSERT(sizeof(sint64) == sizeof(int64_t), PhiOS_ASSERT_MESSAGE);

#endif
