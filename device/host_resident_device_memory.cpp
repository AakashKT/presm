#include "host_resident_device_memory.h"

HostResidentMemory::HostResidentMemory(uint32_t size_in_bytes)
    : DeviceMemory(size_in_bytes)
{
    this->log.log_info("[HostResidentMemory] constructor called");

    this->contents = (char*) malloc(this->size_in_bytes);
}

uint32_t HostResidentMemory::allocate(uint32_t size_in_bytes)
{
    uint32_t address = this->allocation_ptr;
    this->allocation_ptr += size_in_bytes;

    return address;
}

void HostResidentMemory::write(uint32_t start_address, uint32_t num_bytes, const char* data)
{
    for(uint32_t i=0; i<num_bytes; i++)
        this->contents[start_address + i] = data[i];
}

char* HostResidentMemory::read(uint32_t start_address, uint32_t num_bytes)
{
    char* data = (char*) malloc(sizeof(char) * num_bytes);

    for(uint32_t i=0; i<num_bytes; i++)
        data[i] = this->contents[start_address + i];

    return data;
}
