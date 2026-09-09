#include "hw_interface.h"

HwInterface::HwInterface()
    : HwModule(2),
      cp(this)
{
    this->log = new Logger();
    this->log->init("hw_interface");

    this->log->log_info("[HwInterface] Constructor called");
}

std::string HwInterface::module_name()
{
    return "HW_MODULE_INTERFACE";
}

void* HwInterface::get_message_from_device()
{
    return this->get_message(PKT_FROM_DEVICE);
}

void HwInterface::execute_rx()
{
    void* msg = this->get_message(PKT_TO_DEVICE);
    if(msg == nullptr)
        return;

    this->cp.push_message(PKT_TO_CP, msg);
}

void HwInterface::execute_tx()
{
    void* msg = this->get_message(PKT_FROM_CP);
    if(msg == nullptr)
        return;
    
    this->push_message(PKT_FROM_DEVICE, msg);

    this->log->log_info("[HwInterface] Pushing PKT_FROM_DEVICE");
    this->log->log_info(((DevicePayload*)msg)->print());
}

void HwInterface::execute(uint32_t block_idx)
{
    if(block_idx == 0)
        this->execute_rx();
    else if(block_idx == 1)
        this->execute_tx();
}