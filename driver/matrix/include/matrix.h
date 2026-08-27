#ifndef MATRIX_H
#define MATRIX_H

#include <cstdint>

class MIntDeviceMemory {
public:
    MIntDeviceMemory(int source);
    int getValue();

private:
    int size_in_bytes;
    int address;
};

class MCommandInfo {
public:
    MCommandInfo(uint8_t id) : id(id) {};

    uint8_t id;
};

void mInit();
MCommandInfo mAdd(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result);
void mSync(MCommandInfo& info);
void mFree();

#endif