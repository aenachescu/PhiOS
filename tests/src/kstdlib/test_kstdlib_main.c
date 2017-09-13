#include <cut.h>

CUT_INCLUDE_MODULE(module_kstring);
CUT_INCLUDE_MODULE(module_kstdlib);
CUT_INCLUDE_MODULE(module_ksnprintf);
CUT_INCLUDE_MODULE(module_kvsnprintf);
CUT_INCLUDE_MODULE(module_ksnprintf_helper);

CUT_DEFINE_MAIN
    CUT_CALL_MODULE(module_kstring);
    CUT_CALL_MODULE(module_kstdlib);
    CUT_CALL_MODULE(module_ksnprintf);
    CUT_CALL_MODULE(module_kvsnprintf);
    CUT_CALL_MODULE(module_ksnprintf_helper);
CUT_END_MAIN
