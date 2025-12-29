#ifndef PTX_GPU_1_H
#define PTX_GPU_1_H

#include "device.h"

class PTXGPU1 : public Device {
public:
    PTXGPU1();

    std::string get_name() override { return name; };
    DeviceMemory* get_device_memory() override { return device_memory; };

private:
    std::string name;

    DeviceMemory* device_memory;
};

#endif