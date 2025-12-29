#pragma once

#ifndef CUDA_DRIVER_ENTRYPOINT_H
#define CUDA_DRIVER_ENTRYPOINT_H

#include <iostream>
#include <cuda.h>

#include "device.h"

extern Device *presm_device;

CUresult presm_cuInit(unsigned int flags);
CUresult presm_cuDeviceGetCount(int *count);
CUresult presm_cuDeviceGet(CUdevice* device, int ordinal);
CUresult presm_cuDeviceGetName(char* name, int len, CUdevice dev);
CUresult presm_cuDeviceTotalMem(size_t* bytes, CUdevice dev);
CUresult presm_cuCtxCreate(CUcontext* pctx, unsigned int  flags, CUdevice dev);

#endif