#include "sanity_kernel.h"

int add(int a, int b)
{
    return a + b;
}

int subtract(int a, int b)
{
    return a - b;
}

int cond_func(int *a, int *b, int *c, int *d)
{
    int z = 0;
    if(a[0] < b[0])
        z = add(a[0], b[0]);
    else if(a[0] >= b[0])
        z = subtract(a[0], b[0]);

    int i = c[0] + z;

    d[0] = i;
    
    return i;
}