#ifndef PhiOS_PREPROCESSOR
#define PhiOS_PREPROCESSOR

#define CONCAT2(x, y) x ## y
#define CONCAT(x, y) CONCAT2(x, y)

#define UNREFERENCED_PARAMETER(p) (void)p;

#endif
