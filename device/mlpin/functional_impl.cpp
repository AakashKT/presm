#include "functional_impl.h"

Device* get_device()
{
    return new FunctionalImpl();
}

FunctionalImpl::FunctionalImpl()
{
    
}

FunctionalImpl::~FunctionalImpl()
{
    
}

void FunctionalImpl::device_initialize()
{  
    this->log->log_info("Size of DeviceInfoResponse: " + std::to_string(sizeof(DeviceInfoResponse)));
}

uint32_t FunctionalImpl::allocate_device_memory(uint32_t size_in_bytes)
{
    return 0;
}

void FunctionalImpl::write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data)
{

}

char* FunctionalImpl::read_from_device_memory(uint32_t address, uint32_t size_in_bytes)
{
    return nullptr;
}

void FunctionalImpl::send_device_payload(void* payload)
{
    
}

bool FunctionalImpl::receive_device_payload(void *payload)
{
    return false;
}