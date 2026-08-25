#include "device_memory.h"

DeviceMemory::DeviceMemory(uint32_t size_in_bytes)
    : size_in_bytes(size_in_bytes)
{
    this->log.init("device_memory");
    this->log.log_info("DeviceMemory constructor called");

    std::ifstream json_file("hw_config.json");

    this->top_config = nlohmann::json::parse(json_file);
    this->device_config = this->top_config["device"];
}