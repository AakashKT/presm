#include "ptx_gpu_1.h"

PTXGPU1::PTXGPU1()
{
    this->name = "PTX GPU v1";

    this->device_memory = new DeviceMemory(12e9);
}