#ifndef HOST_RESIDENT_DEVICE_MEMORY_H
#define HOST_RESIDENT_DEVICE_MEMORY_H

#include "device_memory.h"

class HostResidentMemory : public DeviceMemory {
public:
    HostResidentMemory(uint32_t size_in_bytes);

    uint32_t allocate(uint32_t size_in_bytes) override;

    void write(uint32_t start_address, uint32_t num_bytes, const uint8_t* data) override;
    uint8_t* read(uint32_t start_address, uint32_t num_bytes) override;

private:
    uint8_t* contents;
    uint32_t allocation_ptr = 0;
};

#endif