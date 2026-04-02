#include "memory.h"

DeviceMemory* get_device_memory()
{
    return new HostResidentMemory(1e10);
}