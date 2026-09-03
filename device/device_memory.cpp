#include "device_memory.h"

DeviceMemory::DeviceMemory(uint32_t size_in_bytes)
    : size_in_bytes(size_in_bytes)
{
    this->log.init("device_memory");
    this->log.log_info("[DeviceMemory] constructor called");
}