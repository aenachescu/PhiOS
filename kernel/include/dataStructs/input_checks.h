#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_INPUT_CHECKS
#define CLIB_TEMPLATE_DATA_STRUCTURES_INPUT_CHECKS

#ifdef DISABLE_INPUT_CHECKS

#define INPUT_CHECK(cond, err)

#else

#define INPUT_CHECK(cond, err)                                                  \
if (cond) {                                                                     \
    return err;                                                                 \
}

#endif // end if DISABLE_INPUT_CHECKS

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_INPUT_CHECKS
