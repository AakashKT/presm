#include "functional_impl.h"

Device* get_device()
{
    return new FunctionalImpl();
}

FunctionalImpl::FunctionalImpl()
    : Device()
{
    this->log->log_info("[FunctionalImpl] 'Preinception Device' constructor called");

    this->device_memory = new HostResidentMemory(this->device_config["memory_size_in_bytes"]);
}

uint32_t FunctionalImpl::allocate_device_memory(uint32_t size_in_bytes)
{
    return this->device_memory->allocate(size_in_bytes);
}

void FunctionalImpl::write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data)
{
    this->device_memory->write(address, size_in_bytes, data);
}

char* FunctionalImpl::read_from_device_memory(uint32_t address, uint32_t size_in_bytes)
{
    return this->device_memory->read(address, size_in_bytes);
}

void FunctionalImpl::send_device_payload(void* payload)
{
    this->hw_uart_packet.push_message(payload);
}

bool FunctionalImpl::receive_device_payload(void **payload)
{

}