#ifndef HOST_RESIDENT_MEMORY_H
#define HOST_RESIDENT_MEMORY_H

#include "memory.h"

// class HostResidentMemory : public DeviceMemory {
// public:
//     HostResidentMemory(uint64_t size_in_bytes)
//         : DeviceMemory(size_in_bytes)
//     {
//         contents = (char*) malloc(this->size_in_bytes);
//     };

//     void initialize() override;
//     void write_byte(uint64_t address, const char data) override;
//     char read_byte(uint64_t address) override;

// private:
//     char* contents;
// };

#endif