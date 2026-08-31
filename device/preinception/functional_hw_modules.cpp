#include "functional_hw_modules.h"

HwUartPacket::HwUartPacket()
    : HwModule()
{
    this->log = new Logger();
    this->log->init("hw_uart_packet");
}

void HwUartPacket::execute()
{
    void* msg = this->get_message();
    if(msg == nullptr)
        return;

    DevicePayload* payload = (DevicePayload*) msg;

    this->log->log_info("Execute Called!");
    this->log->log_info("ID: " + std::to_string(payload->fields.id));
}