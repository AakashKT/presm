#include "driver_entrypoint.h"

CUresult cuInit(unsigned int flags)
{
    return presm_cuInit(flags);
}

// CUresult cuDeviceGetCount(int *count)
// {
//     return presm_cuDeviceGetCount(count);
// }

// CUresult cuDeviceGet(CUdevice* device, int ordinal)
// {
//     return presm_cuDeviceGet(device, ordinal);
// }

// CUresult cuDeviceGetName(char* name, int len, CUdevice dev)
// {
//     return presm_cuDeviceGetName(name, len, dev);
// }

// CUresult cuDeviceTotalMem(size_t* bytes, CUdevice dev)
// {
//     return presm_cuDeviceTotalMem(bytes, dev);
// }

// CUresult cuCtxCreate(CUcontext* pctx, CUctxCreateParams* ctxCreateParams, 
//     unsigned int  flags, CUdevice dev)
// {

// }