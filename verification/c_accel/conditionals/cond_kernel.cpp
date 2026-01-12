#include "cond_kernel.h"

int add_cond(int a, int b)
{
    return a + b;
}

int subtract_cond(int a, int b)
{
    return a - b;
}

int conditionals(int *a, int *b, int *c, int *d)
{
    int z = 0;
    if(a[0] < b[0])
        z = add_cond(a[0], b[0]);
    else if(a[0] >= b[0])
        z = subtract_cond(a[0], b[0]);

    int i = c[0] + z;

    d[0] = i;
    
    return i;
}