#ifndef HOST_RESIDENT_DEVICE_MEMORY_H
#define HOST_RESIDENT_DEVICE_MEMORY_H

#include "device_memory.h"

class HostResidentMemory : public DeviceMemory {
public:
    HostResidentMemory(uint64_t size_in_bytes);

    void write(uint64_t start_address, uint64_t num_bytes, const char* data) override;
    char* read(uint64_t start_address, uint64_t num_bytes) override;

private:
    char* contents;
};

#endif