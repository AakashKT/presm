#ifndef DEVICE_H
#define DEVICE_H

#include "common.h"
#include "memory.h"

class Device {
public:
    virtual std::string get_name() = 0;

    virtual DeviceMemory* get_device_memory() = 0;
};

Device* initialize_device();

#endif