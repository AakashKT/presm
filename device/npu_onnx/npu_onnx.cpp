#include "npu_onnx.h"

NPUONNX::NPUONNX()
{
    this->name = "Neural Processing Unit (NPU) for ONNX driver";

    this->device_memory = new DeviceMemory(12e9);
}