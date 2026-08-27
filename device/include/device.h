#ifndef DEVICE_H
#define DEVICE_H

#include "common.h"
#include "logging.h"
#include "device_memory.h"
#include <nlohmann/json.hpp>

class Device {
public:
    Device();
    virtual ~Device() {};
    
    std::string get_name() { return std::string(this->device_config["name"]); };

    virtual void device_initialize() = 0;

    virtual void send_device_packet(void* packet, uint32_t size_in_bytes) = 0;

    virtual uint32_t allocate_device_memory(uint32_t size_in_bytes) = 0;
    virtual void write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data) = 0;
    virtual char* read_from_device_memory(uint32_t address, uint32_t size_in_bytes) = 0;

protected:
    Logger* log;
    nlohmann::json top_config, device_config;

    DeviceMemory* device_memory;
};

Device* get_device();

#endif