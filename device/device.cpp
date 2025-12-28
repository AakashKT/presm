#include "device.h"

#if DEIVCE_ID==0
#include "ptx_gpu_1.h"
#endif

PRESMDevice* initialize_device()
{
#if DEIVCE_ID==0
    return new PTXGPU1();
#else
    return nullptr;
#endif
}