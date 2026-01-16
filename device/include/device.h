#ifndef DEVICE_H
#define DEVICE_H

#include "common.h"
#include "memory.h"

class Device {
public:
    virtual std::string get_name() = 0;

public:
    DeviceMemory* device_memory;
};

Device* get_device();

#endif