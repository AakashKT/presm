#include "add_func.cuh"

extern "C" __global__ void vector_add(int *a, int *b, int *c, int N)
{
    int tid = blockIdx.x;
    if (tid < N)
        c[tid] = add_fun(a[tid], b[tid]);
}