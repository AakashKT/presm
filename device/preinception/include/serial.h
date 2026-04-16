#ifndef SERIAL_H
#define SERIAL_H

#include "serial_device.h"
#include "host_resident_device_memory.h"

class SerialImpl : public SerialDevice {
public:
    SerialImpl();

    void serial_read_process(char data) override;

private:
    uint32_t temp_count = 0;
};

#endif