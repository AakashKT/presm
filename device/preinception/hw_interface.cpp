#include "hw_interface.h"

HwInterface::HwInterface(HwClock& clk)
    : HwModule<DevicePayload>(2, clk),
      cp(this, clk)
{
    this->log = new Logger();
    this->log->init("hw_interface");

    this->log->log_info("[HwInterface] Constructor called");
}

std::string HwInterface::module_name()
{
    return "HW_MODULE_INTERFACE";
}

HwMessage<DevicePayload> HwInterface::get_message_from_device()
{
    HwMessage<DevicePayload> rval;
    rval.ack = true;
    rval.data = nullptr;

    auto payload = this->driver_payload.pop_front();
    if(payload != std::nullopt)
        rval = *payload;

    return rval;
}

void HwInterface::execute_rx()
{
    HwMessage<DevicePayload> msg = this->get_message(PKT_TO_DEVICE);
    if(msg.data == nullptr || msg.ack == true)
        return;

    this->cp.push_message(PKT_TO_CP, msg.data);

    this->log->log_info("[HwInterface] Pushing PKT_TO_CP");
    this->log->log_info(((DevicePayload*)msg.data.get())->print());
}

void HwInterface::execute_tx()
{
    HwMessage<DevicePayload> msg = this->get_message(PKT_FROM_CP);
    if(msg.data == nullptr || msg.ack == true)
        return;
    
    this->driver_payload.push_back(msg);

    this->log->log_info("[HwInterface] Pushing driver payload");
    this->log->log_info(((DevicePayload*)msg.data.get())->print());
}

void HwInterface::execute(uint32_t block_idx)
{
    if(block_idx == 0)
        this->execute_rx();
    else if(block_idx == 1)
        this->execute_tx();
}