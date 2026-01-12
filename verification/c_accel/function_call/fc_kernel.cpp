#include "fc_kernel.h"

int add_fc(int a, int b)
{
    return a + b;
}

int subtract_fc(int a, int b)
{
    return a - b;
}

int func_calls(int *a, int *b, int *c, int *d)
{
    int z = add_fc(a[0], b[0]);
    int i = subtract_fc(c[0], z);

    d[0] = i;
    
    return i;
}