#include "ptx_gpu.h"

Device* get_device()
{
    return new PTXGPU1();
}

PTXGPU1::PTXGPU1()
{
    this->name = "PTX GPU v1";

    this->device_memory = new DeviceMemory(12e9);
}