#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include "memory.h"

class PRESMDevice {
public:
    virtual std::string get_name() = 0;

    virtual DeviceMemory* get_device_memory() = 0;
};

PRESMDevice* initialize_device();

#endif