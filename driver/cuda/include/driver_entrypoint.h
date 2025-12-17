#ifndef CUDA_DRIVER_ENTRYPOINT_H
#define CUDA_DRIVER_ENTRYPOINT_H

#include <iostream>
#include <cuda.h>

CUresult presm_cuInit(unsigned int flags);
CUresult presm_cuDeviceGetCount(int *count);

#endif