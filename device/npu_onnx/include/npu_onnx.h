#ifndef NPU_ONNX_H
#define NPU_ONNX_H

#include "device.h"

class NPUONNX : public Device {
public:
    NPUONNX();

    std::string get_name() override { return name; };
    DeviceMemory* get_device_memory() override { return device_memory; };

private:
    std::string name;

    DeviceMemory* device_memory;
};

#endif