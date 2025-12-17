#include "driver_entrypoint.h"

CUresult presm_cuInit(unsigned int flags)
{
    std::cout << "cuInit() of driver" << std::endl;
    return CUDA_SUCCESS;
}

CUresult presm_cuDeviceGetCount(int *count)
{
    *count = 4;
    return CUDA_SUCCESS;
}