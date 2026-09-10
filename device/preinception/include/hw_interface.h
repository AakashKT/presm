#ifndef HW_INTERFACE_H
#define HW_INTERFACE_H

#include "hw_module.h"
#include "hw_cp.h"
#include "defs.h"

class HwInterface : public HwModule<DevicePayload> {
public:
    HwInterface();

    void execute_rx();
    void execute_tx();

    HwMessage<DevicePayload> get_message_from_device();
    
    std::string module_name() override;
    void execute(uint32_t block_idx) override;

private:
    Logger* log;
    HwCp cp;

    ThreadSafeList<HwMessage<DevicePayload>> driver_payload;
};

#endif