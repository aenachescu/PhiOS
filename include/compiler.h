#ifndef PhiOS_COMPILER
#define PhiOS_COMPILER

#ifdef __GNUC__

#define PhiOS_PACKED_STRUCTURE  __attribute__ ((packed))
#define PhiOS_ALWAYS_INLINE     __attribute__ ((always_inline))

#else

#error "unsupported compiler"

#endif

#endif
