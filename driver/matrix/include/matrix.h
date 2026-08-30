#ifndef MATRIX_H
#define MATRIX_H

#include <cstdint>

class MUIntDeviceMemory {
public:
    MUIntDeviceMemory(uint32_t source);
    uint32_t getValue();

    uint32_t size_in_bytes;
    uint32_t address;
};

class MIntDeviceMemory {
public:
    MIntDeviceMemory(int32_t source);
    int32_t getValue();

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
MCommandInfo mAdd(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result);
void mSync(MCommandInfo& info);
void mFree();

#endif