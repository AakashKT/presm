#include "driver_entrypoint.h"

CUresult cuInit(unsigned int flags)
{
    return presm_cuInit(flags);
}

CUresult cuDeviceGetCount(int *count)
{
    return presm_cuDeviceGetCount(count);
}