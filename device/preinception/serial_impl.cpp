#include "serial_impl.h"

Device* get_device()
{
    return new SerialImpl();
}

SerialImpl::SerialImpl()
    : SerialDevice()
{
    this->device_initialize();
    this->device_memory = new HostResidentMemory(this->device_config["memory_size_in_bytes"]);
}

void SerialImpl::serial_read_process(uint8_t data)
{
    this->log->log_info("Received from device: " + std::to_string(data));
}

uint32_t SerialImpl::allocate_device_memory(uint32_t size_in_bytes)
{
    return this->device_memory->allocate(size_in_bytes);
}

char* SerialImpl::read_from_device_memory(uint32_t address, uint32_t size_in_bytes)
{
    return this->device_memory->read(address, size_in_bytes);
}

void SerialImpl::write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data)
{
    this->device_memory->write(address, size_in_bytes, data);
}