#include "device.h"

Device::Device()
{
    this->log = new Logger();
    this->log->init("device");

    std::ifstream json_file("hw_config.json");

    this->top_config = nlohmann::json::parse(json_file);
    this->device_config = this->top_config["device"];

    this->log->log_info("[Device] Constructor called");
}