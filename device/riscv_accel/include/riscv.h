#ifndef RISCV_ACCEL_H
#define RISCV_ACCEL_H

#include "device.h"

class RISCVAccel : public Device {
public:
    RISCVAccel();

    std::string get_name() override { return name; };
    DeviceMemory* get_device_memory() override { return device_memory; };

private:
    std::string name;

    DeviceMemory* device_memory;
};

#endif