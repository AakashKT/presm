#ifndef DEVICE_H
#define DEVICE_H

#include "common.h"
#include "logging.h"
#include <nlohmann/json.hpp>

class Device {
public:
    Device();
    
    std::string get_name() { return std::string(this->device_config["name"]); };

private:
    nlohmann::json top_config, device_config;

protected:
    Logger device_log;
};

Device* get_device();

#endif