#include "driver_entrypoint.h"

PRESMDevice *presm_device = nullptr;

CUresult presm_cuInit(unsigned int flags)
{
    presm_device = initialize_device();
    return CUDA_SUCCESS;
}

CUresult presm_cuDeviceGetCount(int *count)
{
    if(presm_device == nullptr)
        return CUDA_ERROR_NOT_INITIALIZED;

    *count = 1;
    return CUDA_SUCCESS;
}

CUresult presm_cuDeviceGet(CUdevice* device, int ordinal)
{
    if(presm_device == nullptr)
        return CUDA_ERROR_NOT_INITIALIZED;

    if(ordinal > 0)
        return CUDA_ERROR_INVALID_DEVICE;

    *device = reinterpret_cast<int64_t>(presm_device) >> 32;

    return CUDA_SUCCESS;
}

CUresult presm_cuDeviceGetName(char* name, int len, CUdevice dev)
{
    if(presm_device == nullptr)
        return CUDA_ERROR_NOT_INITIALIZED;

    if((reinterpret_cast<int64_t>(presm_device) >> 32) != dev)
        return CUDA_ERROR_INVALID_DEVICE;
    
    std::string device_name = presm_device->get_name();
    const char* device_name_c_str = device_name.c_str();
    for(int i=0; i<len;i++)
        name[i] = device_name_c_str[i];

    return CUDA_SUCCESS;
}

CUresult presm_cuDeviceTotalMem(size_t* bytes, CUdevice dev)
{
    if(presm_device == nullptr)
        return CUDA_ERROR_NOT_INITIALIZED;

    if((reinterpret_cast<int64_t>(presm_device) >> 32) != dev)
        return CUDA_ERROR_INVALID_DEVICE;

    *bytes = (size_t) presm_device->get_device_memory()->get_size();

    return CUDA_SUCCESS;
}

CUresult presm_cuCtxCreate(CUcontext* pctx, unsigned int  flags, CUdevice dev)
{
    return CUDA_ERROR_NOT_INITIALIZED;
}