#include "serial.h"

Device* get_device()
{
    return new SerialImpl();
}

SerialImpl::SerialImpl()
{
    this->device_initialize();
    this->device_memory = new HostResidentMemory(this->device_config["memory_size_in_bytes"]);
}

void SerialImpl::serial_read_process(char data)
{
    char temp[2] = {data, '\0'};
    this->log.log_info(std::string(temp));
}