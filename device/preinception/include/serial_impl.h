#ifndef SERIAL_H
#define SERIAL_H

#include "serial_device.h"
#include "host_resident_device_memory.h"
#include "defs.h"

class SerialImpl : public SerialDevice {
public:
    SerialImpl();

    void serial_read_process(uint8_t data) override;
    void device_find() override;

    void send_device_payload(void* payload) override;
    bool receive_device_payload(void **payload) override;

    uint32_t allocate_device_memory(uint32_t size_in_bytes) override;
    void write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data) override;
    char* read_from_device_memory(uint32_t address, uint32_t size_in_bytes) override;

private:
    std::queue<DevicePayload> received_payloads;
    DevicePayload scratch;
    uint32_t scratch_ptr = 0, scratch_ptr_max = 8;
};

#endif