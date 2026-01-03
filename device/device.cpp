#include "device.h"

#if DEVICE_ID==0
#include "ptx_gpu.h"
#endif

Device* initialize_device()
{
#if DEVICE_ID==0
    return new PTXGPU1();
#endif

    return nullptr;
}