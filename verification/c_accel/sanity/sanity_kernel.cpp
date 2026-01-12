#include "sanity_kernel.h"

int sanity_kernel(int* a, int* b, int* c, int* d)
{
    d[0] = a[0] + b[0] - c[0];
    return d[0];
}