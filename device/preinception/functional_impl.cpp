#include "functional_impl.h"

Device* get_device()
{
    return new FunctionalImpl();
}

FunctionalImpl::FunctionalImpl()
    : Device()
{
    this->log->log_info("[FunctionalImpl] 'Preinception Device' constructor called");
    this->device_memory = new HostResidentMemory(std::atoi(HOST_RESIDENT_MEM_SIZE));
}

void FunctionalImpl::device_initialize()
{
    bool found = false;

    DevicePayload tx;
    tx.id(1);
    tx.type((uint32_t)TYPE::REQUEST);
    tx.cmd((uint32_t)CMD::HANDSHAKE);
    tx.sub_cmd((uint32_t)HANDSHAKE::OP);
    tx.fields32.body = 0;
    this->send_device_payload(&tx);

    auto begin_time = std::chrono::high_resolution_clock::now();
    while(true) {
        DevicePayload* rx = (DevicePayload*) this->hw_interface.get_message_from_device();
        
        if(rx != nullptr) {
            if(rx->id() == 1 && rx->type() == static_cast<uint32_t>(TYPE::RESPONSE)
                && rx->fields.body_1 == 2 && rx->fields.body_2 == 1) {
                this->log->log_info("[FunctionalImpl] Found device.");
                found = true;
            }
            else {
                this->log->log_info("[FunctionalImpl] Received packet does not match handshake signature");
                this->log->log_info(rx->print());
            }
        }

        auto time_diff = std::chrono::high_resolution_clock::now() - begin_time;
        if(found || std::chrono::duration_cast<std::chrono::microseconds>(time_diff).count() >= 2e6)
            break;
    }

    if(!found)
        this->log->log_error_and_exit("[SerialImpl] Could not find device over serial port.");
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
    this->hw_interface.push_message(PKT_TO_DEVICE, payload);

    this->log->log_info("[FunctionalImpl] Sent device payload ->");
    this->log->log_info(((DevicePayload*)payload)->print());
}

bool FunctionalImpl::receive_device_payload(void **payload)
{
    void* msg = this->hw_interface.get_message_from_device();

    if(msg == nullptr)
        return false;

    *payload = msg;

    this->log->log_info("[FunctionalImpl] Received device payload ->");
    this->log->log_info(((DevicePayload*)msg)->print());

    return true;
}