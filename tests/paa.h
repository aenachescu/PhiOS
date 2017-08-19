#include <cut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kernel/include/memory/paa.h"
#include "errors.h"

void *paa_memory;

CUT_DEFINE_TEST(test_PAA_init)
{
    uint32 err;
    paa_memory = malloc(0x1000 * 25);

    err = PAA_init((size_t) paa_memory);
    CUT_CHECK(err == ERROR_SUCCESS);
}

CUT_DEFINE_TEST(test_PAA_alloc)
{
    uint32 err;

    size_t addr;
    size_t addr1;
    size_t addr2;
    int *x;
    char *s;

    err = PAA_alloc(0, &addr, 0);
    CUT_CHECK(err == ERROR_INVALID_PARAMETER);

    err = PAA_alloc(10, NULL, 0);
    CUT_CHECK(err == ERROR_NULL_POINTER);

    CUT_CHECK(PAA_alloc(sizeof(int), &addr, 0x1000) == ERROR_SUCCESS);
    CUT_CHECK((addr & (0x1000 - 1)) == 0);

    CUT_CHECK(PAA_alloc(sizeof(int), &addr1, 1) == ERROR_SUCCESS);
    CUT_CHECK(PAA_alloc(sizeof(char) * 20, &addr2, 1) == ERROR_SUCCESS);

    x = (int*) addr1;
    s = (char*) addr2;

    *x = 5;
    strcpy(s, "This is so nice!\n");
    CUT_CHECK(*x == 5);
    CUT_CHECK(!strcmp(s, "This is so nice!\n"));
    free(paa_memory);
}