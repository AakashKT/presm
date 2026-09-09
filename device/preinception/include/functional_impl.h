#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include "device.h"
#include "host_resident_device_memory.h"
#include "hw_interface.h"
#include "defs.h"
#include "config_preinception.h"

class FunctionalImpl : public Device {
public:
    FunctionalImpl();

    void device_initialize();

    void send_device_payload(void* payload) override;
    bool receive_device_payload(void **payload) override;

    uint32_t allocate_device_memory(uint32_t size_in_bytes) override;
    void write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data) override;
    char* read_from_device_memory(uint32_t address, uint32_t size_in_bytes) override;

private:
    HwInterface hw_interface;
};

#endif