#ifndef PTX_GPU_1
#define PTX_GPU_1

#include "device.h"

class PTXGPU1 : public PRESMDevice {
public:
    PTXGPU1();

    std::string get_name() override { return name; };
    DeviceMemory* get_device_memory() override { return device_memory; };

private:
    std::string name;

    DeviceMemory* device_memory;
};

#endif