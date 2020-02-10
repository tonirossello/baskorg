#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

int suma(int a, int b)
{
    return a + b;
}

TEST_CASE("Test suma()")
{
    CHECK ( suma(1,2) == 3);
}