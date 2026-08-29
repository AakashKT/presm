#ifndef DEVICE_MEMORY_H
#define DEVICE_MEMORY_H

#include "common.h"
#include "logging.h"
#include <nlohmann/json.hpp>

class DeviceMemory {
public:
    DeviceMemory(uint32_t size_in_bytes);

    uint32_t get_size() { return size_in_bytes; };

    virtual uint32_t allocate(uint32_t size_in_bytes) = 0;

    virtual void write(uint32_t start_address, uint32_t num_bytes, const uint8_t* data) = 0;
    virtual uint8_t* read(uint32_t start_address, uint32_t num_bytes) = 0;

protected:
    uint32_t size_in_bytes; 
    
    Logger log;
    nlohmann::json top_config, device_config;
};

#endif