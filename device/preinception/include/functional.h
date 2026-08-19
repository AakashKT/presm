#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include "device.h"

class Functional : public Device {
public:
    Functional();

    void device_initialize() {};

    void send_device_packet(uint32_t size_in_bytes, char* packet) {};

    uint32_t allocate_device_memory(uint32_t size_in_bytes) { return 0; };
    char* read_from_device_memory(uint32_t address, uint32_t size_in_bytes) { return nullptr; };
    void write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data) {};

private:

};

#endif