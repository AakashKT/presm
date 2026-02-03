#include "device.h"

Device::Device()
{
    this->device_log.init("device");

    this->device_log.log_info("Device constructor called");

    std::ifstream json_file("presm_config.json");

    this->top_config = nlohmann::json::parse(json_file);
    this->device_config = this->top_config["device"];
}