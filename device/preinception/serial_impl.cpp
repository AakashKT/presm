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

void SerialImpl::send_device_payload(void* payload)
{
    DevicePayload* sc = (DevicePayload*) payload;

    usleep(SERIAL_WRITE_DELAY);
    auto bytes_written = write(this->port_fd, (uint8_t*)sc->packet, 8);
    if (bytes_written != 8)
        this->log->log_error_and_exit("[SerialImpl] Failed to send device payload");

    this->log->log_info("[SerialImpl] Sent device payload ->");
    this->log->log_info(sc->print());
}

bool SerialImpl::receive_device_payload(void **payload)
{
    auto sc = this->received_payloads.pop_front();

    if(sc == std::nullopt)
        return false;
    else {
        DevicePayload* rval = (DevicePayload*) malloc(sizeof(DevicePayload));
        *rval = *sc;
        *payload = rval;

        this->log->log_info("[SerialImpl] Received device payload ->");
        this->log->log_info(rval->print());

        return true;
    }
}

uint32_t SerialImpl::allocate_device_memory(uint32_t size_in_bytes)
{
    return this->device_memory->allocate(size_in_bytes);
}

void SerialImpl::write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const uint8_t* data)
{
    this->device_memory->write(address, size_in_bytes, data);
}

uint8_t* SerialImpl::read_from_device_memory(uint32_t address, uint32_t size_in_bytes)
{
    return this->device_memory->read(address, size_in_bytes);
}

void SerialImpl::process_mem_request(DevicePayload& payload)
{
    if(payload.fields.sub_cmd == 0) {\
        this->log->log_info("[SerialImpl] Device requested read, payload ->");
        this->log->log_info(payload.print());

        uint8_t* mem_val = this->read_from_device_memory(payload.fields32.body, 4);

        DevicePayload mem_response;
        mem_response.fields.id = payload.fields.id;
        mem_response.fields.type = 1;
        mem_response.fields.cmd = payload.fields.cmd;
        mem_response.fields.sub_cmd = payload.fields.sub_cmd;
        mem_response.fields.body_1 = mem_val[0];
        mem_response.fields.body_2 = mem_val[1];
        mem_response.fields.body_3 = mem_val[2];
        mem_response.fields.body_4 = mem_val[3];
        this->send_device_payload(&mem_response);
    }
    else if(payload.fields.sub_cmd == 1) {
        if(this->mem_write_state == ADDR_RECV) {
            this->log->log_info("[SerialImpl] Device requested write to address, payload ->");
            this->log->log_info(payload.print());

            this->mem_write_addr_scratch = payload.fields32.body;
            this->mem_write_state = VAL_RECV;
        }
        else if(this->mem_write_state == VAL_RECV) {
            this->log->log_info("[SerialImpl] Device requested write value to above address, payload ->");
            this->log->log_info(payload.print());

            uint8_t data[4] = { payload.fields.body_1, payload.fields.body_2, payload.fields.body_3, payload.fields.body_4 };
            this->write_to_device_memory(this->mem_write_addr_scratch, 4, data);

            DevicePayload mem_response;
            mem_response.fields.id = payload.fields.id;
            mem_response.fields.type = 1;
            mem_response.fields.cmd = payload.fields.cmd;
            mem_response.fields.sub_cmd = payload.fields.sub_cmd;
            mem_response.fields32.body = 0;
            this->send_device_payload(&mem_response);

            this->mem_write_state = ADDR_RECV;
        }
    }
}

void SerialImpl::serial_read_process(uint8_t data)
{
    this->scratch.packet[this->scratch_ptr++] = data;

    if(this->scratch_ptr == 8) {
        if(this->scratch.fields.cmd == 0) 
            this->process_mem_request(this->scratch);
        else {
            this->received_payloads.push_back(this->scratch);
        }

        this->scratch_ptr = 0;
    }
}

void SerialImpl::device_find()
{
    std::string port_string_base = "/dev/ttyUSB";
    bool found = false;

    for(int i=0; i<5; i++) {
        std::string port_string = port_string_base + std::to_string(i);

        bool opened = this->open_serial_port(port_string);
        if(!opened) {
            this->log->log_info("[SerialImpl] Failed to open serial port '" + port_string + "'.");
            continue;
        }

        this->configure_serial_port(this->device_config["fpga"]["baud_rate"]);
        usleep(SERIAL_WRITE_DELAY);
        tcflush(this->port_fd, TCIOFLUSH);

        DevicePayload tx;
        tx.fields.id = 1;
        tx.fields.type = 0;
        tx.fields.cmd = 1;
        tx.fields.sub_cmd = 0;
        tx.fields32.body = 0;
        this->send_device_payload(&tx);

        DevicePayload rx;
        uint32_t total_bytes_read = 0;
        auto begin_time = std::chrono::high_resolution_clock::now();
        uint8_t rx_ptr = 0;
        while(true) {
            ssize_t bytes_read = read(this->port_fd, &rx.packet[rx_ptr], 1);

            if(bytes_read == 1) {
                rx_ptr += 1;
                total_bytes_read += 1;

                if(total_bytes_read == 8) {
                    if(rx.fields.id == 1 && rx.fields.type == 1 && rx.fields.body_1 == 2 && rx.fields.body_2 == 1) {
                        this->log->log_info("[SerialImpl] Found device in serial port '" + port_string + "'.");
                        found = true;
                    }

                    break;
                }
            }

            auto time_diff = std::chrono::high_resolution_clock::now() - begin_time;
            if(found || std::chrono::duration_cast<std::chrono::microseconds>(time_diff).count() >= 2e6)
                break;
        }

        if(found)
            break;
        else {
            tcflush(this->port_fd, TCIOFLUSH);
            close(this->port_fd);
        }
    }

    if(!found)
        this->log->log_error_and_exit("[SerialImpl] Could not find device over serial port.");

    usleep(SERIAL_WRITE_DELAY);
}