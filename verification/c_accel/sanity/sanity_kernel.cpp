#include "sanity_kernel.h"

int fma_func(int a, int b, int c)
{
    int i = a * b;
    int z = i + c;

    return z;
}