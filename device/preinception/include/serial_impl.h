#ifndef SERIAL_H
#define SERIAL_H

#include "serial_device.h"
#include "host_resident_device_memory.h"
#include "defs.h"

enum MemWriteState {
    ADDR_RECV = 0,
    VAL_RECV
};

class SerialImpl : public SerialDevice {
public:
    SerialImpl();

    void process_mem_request(DevicePayload& payload);

    void serial_read_process(uint8_t data) override;
    void device_find() override;

    void send_device_payload(void* payload) override;
    bool receive_device_payload(void **payload) override;

    uint32_t allocate_device_memory(uint32_t size_in_bytes) override;
    void write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const uint8_t* data) override;
    uint8_t* read_from_device_memory(uint32_t address, uint32_t size_in_bytes) override;

private:
    ThreadSafeList<DevicePayload> received_payloads;
    DevicePayload scratch;
    uint32_t scratch_ptr = 0, scratch_ptr_max = 8;

    MemWriteState mem_write_state = ADDR_RECV;
    uint32_t mem_write_addr_scratch;
};

#endif