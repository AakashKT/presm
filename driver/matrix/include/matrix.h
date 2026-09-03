#ifndef MATRIX_H
#define MATRIX_H

#include <cstdint>
#include <cmath>

class MIntDeviceMemory {
public:
    MIntDeviceMemory(int32_t source);
    int32_t getValue();
    void setValue(int32_t new_val);

    uint32_t size_in_bytes;
    uint32_t address;
};

void mInit();
void mAdd(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result);
void mMulPow2(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result);
void mDivPow2(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result);
void mSync();
void mFree();

#endif