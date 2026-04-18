#ifndef SERIAL_H
#define SERIAL_H

#include "serial_device.h"
#include "host_resident_device_memory.h"

class SerialImpl : public SerialDevice {
public:
    SerialImpl();

    void serial_read_process(char data) override;
    
    uint32_t allocate_device_memory(uint32_t size_in_bytes) override;
    char* read_from_device_memory(uint32_t address, uint32_t size_in_bytes) override;
    void write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data) override;

private:
};

#endif