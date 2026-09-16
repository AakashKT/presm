#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include "device.h"
#include "host_resident_device_memory.h"
#include "hw_interface.h"
#include "defs.h"
#include "config_preinception.h"

enum MemWriteState {
    ADDR_RECV = 0,
    VAL_RECV
};

class FunctionalImpl : public Device {
public:
    FunctionalImpl();
    ~FunctionalImpl();

    void device_initialize() override;
    void device_deinitialize() override {};

    void send_device_payload(void* payload) override;
    bool receive_device_payload(void *payload) override;

    void process_device_request(DevicePayload& payload);

    uint32_t allocate_device_memory(uint32_t size_in_bytes) override;
    void write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data) override;
    char* read_from_device_memory(uint32_t address, uint32_t size_in_bytes) override;

private:
    HwInterface hw_interface;

    ThreadSafeList<DevicePayload> received_payloads;
    std::thread device_receive_thread;

    MemWriteState mem_write_state = ADDR_RECV;
    uint32_t mem_write_addr_scratch;

#if DEVICE_DEBUG
    Logger *stats_log;
    ThreadSafeList<DevicePayload> last_request_pkt, last_response_pkt;
#endif

};

#endif