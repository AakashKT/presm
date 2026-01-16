#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

class DeviceMemory {
public:
    DeviceMemory(uint64_t size_in_bytes);

    uint64_t get_size() { return size_in_bytes; };

    void write_request(uint64_t start_address, uint64_t num_bytes, const char* data);
    void read_request(uint64_t start_address, uint64_t num_bytes);

private:
    virtual void write_byte(uint64_t address, const char data) = 0;
    virtual char read_byte(uint64_t address) = 0;

protected:
    uint64_t size_in_bytes; 
    
};

#endif