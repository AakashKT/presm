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
    // this->device_memory.write(this->temp_count % 4, 1, &data);
    // this->temp_count++;
}