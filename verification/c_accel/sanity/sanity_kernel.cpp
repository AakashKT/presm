#include "sanity_kernel.h"

int add(int a, int b)
{
    return a + b;
}

int subtract(int a, int b)
{
    return a - b;
}

int fsa_func(int *a, int *b, int *c, int *d)
{
    int i = subtract(a[0], b[0]);
    int z = add(i, c[0]);

    d[0] = z;
    
    return z;
}