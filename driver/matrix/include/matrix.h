#ifndef MATRIX_H
#define MATRIX_H

#include <cstdint>

class MUIntDeviceMemory {
public:
    MUIntDeviceMemory(int source);
    uint32_t getValue();

    uint32_t size_in_bytes;
    uint32_t address;
};

class MCommandInfo {
public:
    MCommandInfo(uint8_t id) : id(id) {};

    uint8_t id;
};

void mInit();
MCommandInfo mAdd(MUIntDeviceMemory& first, MUIntDeviceMemory& second, MUIntDeviceMemory& result);
void mSync(MCommandInfo& info);
void mFree();

#endif