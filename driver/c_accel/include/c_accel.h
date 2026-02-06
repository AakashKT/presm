#pragma once

#ifndef C_ACCEL_H
#define C_ACCEL_H

#include <cstddef>
#include <string>

class caDeviceBuffer {
public:
    caDeviceBuffer(size_t size_in_bytes);

private:
    size_t size_in_bytes;
};

void caDeviceInit();
uint32_t caAdd(uint32_t a, uint32_t b);

void caSetKernel(std::string kernel_file);
void caCopyHtoD(void* src, caDeviceBuffer& dest, size_t size_in_bytes);
void caCopyDtoH(caDeviceBuffer& src, void* dest, size_t size_in_bytes);
void caSetKernelParams(uint32_t param_loc, caDeviceBuffer& buffer);
void caKernelRun();
void caSync();
void* caGetKernelOutput();

void caDebugCheckDisassembly();

#endif