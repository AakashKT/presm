#ifndef DEVICE_H
#define DEVICE_H

#include "common.h"
#include "logging.h"
#include "device_memory.h"
#include <nlohmann/json.hpp>

class Device {
public:
    Device();
    
    std::string get_name() { return std::string(this->device_config["name"]); };

    virtual void device_initialize() = 0;

protected:
    Logger log;
    nlohmann::json top_config, device_config;

    DeviceMemory* device_memory;
};

Device* get_device();

#endif