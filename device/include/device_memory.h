#ifndef DEVICE_MEMORY_H
#define DEVICE_MEMORY_H

#include "common.h"
#include "logging.h"
#include <nlohmann/json.hpp>

class DeviceMemory {
public:
    DeviceMemory(uint64_t size_in_bytes);

    uint64_t get_size() { return size_in_bytes; };

    virtual void write(uint64_t start_address, uint64_t num_bytes, const char* data) = 0;
    virtual char* read(uint64_t start_address, uint64_t num_bytes) = 0;

//     void write_request(uint64_t start_address, uint64_t num_bytes, const char* data);
//     void read_request(uint64_t start_address, uint64_t num_bytes);

// private:
//     virtual void write_byte(uint64_t address, const char data) = 0;
//     virtual char read_byte(uint64_t address) = 0;

protected:
    uint64_t size_in_bytes; 
    
    Logger log;
    nlohmann::json top_config, device_config;
};

DeviceMemory* get_device_memory();

#endif