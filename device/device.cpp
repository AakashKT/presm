#include "device.h"

#if DEVICE_ID==0
#include "ptx_gpu.h"
#endif

#if DEVICE_ID==1
#include "npu_onnx.h"
#endif

Device* initialize_device()
{
#if DEVICE_ID==0
    return new PTXGPU1();
#endif

#if DEVICE_ID==1
    return new NPUONNX();
#endif

    return nullptr;
}