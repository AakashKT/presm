#include "hw_cp.h"

HwCp::HwCp(HwModule* hw_interface_module)
    : HwModule(1),
      hw_interface_module(hw_interface_module)
{
    this->log = new Logger();
    this->log->init("hw_cp");

    this->log->log_info("[HwCp] Constructor called");
}

std::string HwCp::module_name()
{
    return "HW_MODULE_CP";
}

void HwCp::execute(uint32_t block_idx)
{
    DevicePayload* msg = (DevicePayload*) this->get_message(PKT_TO_CP);
    if(msg == nullptr)
        return;

    // Handshake
    if(msg->type() == 0 && msg->cmd() == 1 && msg->sub_cmd() == 0) {
        this->log->log_info("[HwCp] Handshake Requested");

        DevicePayload* handshake = (DevicePayload*) malloc(sizeof(DevicePayload));
        handshake->id(msg->id());
        handshake->type((uint32_t)TYPE::RESPONSE);
        handshake->cmd(msg->cmd());
        handshake->sub_cmd(msg->sub_cmd());
        handshake->fields.body_1 = 2;
        handshake->fields.body_2 = 1;
        handshake->fields.body_3 = 0;
        handshake->fields.body_4 = 0;

        this->hw_interface_module->push_message(PKT_FROM_CP, handshake);
    }
    else {
        this->log->log_info("[HwCp] Unknown packet");
    }
}