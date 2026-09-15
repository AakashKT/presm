#ifndef SERIAL_H
#define SERIAL_H

#include "serial_device.h"
#include "host_resident_device_memory.h"
#include "defs.h"
#include "config_preinception.h"

enum DevicePacketRecvState {
    ADDR_RECV = 0,
    VAL_RECV
};

class SerialImpl : public SerialDevice {
public:
    SerialImpl();
    ~SerialImpl();

    void process_device_request(DevicePayload& payload);

    void serial_read_process(char data) override;
    void device_find() override;

    void send_device_payload(void* payload) override;
    bool receive_device_payload(void *payload) override;

    uint32_t allocate_device_memory(uint32_t size_in_bytes) override;
    void write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data) override;
    char* read_from_device_memory(uint32_t address, uint32_t size_in_bytes) override;

private:
    ThreadSafeList<DevicePayload> received_payloads;
    DevicePayload scratch;
    uint32_t scratch_ptr = 0;

    DevicePacketRecvState device_packet_recv_state = ADDR_RECV;
    uint32_t mem_write_addr_scratch;

#if DEVICE_DEBUG
    Logger *stats_log;
    ThreadSafeList<DevicePayload> last_request_pkt, last_response_pkt;
#endif

};

#endif