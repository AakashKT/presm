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

    auto bytes_written = write(this->port_fd, (uint8_t*)sc->packet, 4 + sc->fields.num_bytes);
    if (bytes_written != 4)
        this->log->log_error_and_exit("Failed to send device payload.");
}

bool SerialImpl::receive_device_payload(void **payload)
{
    if(this->received_payloads.size() == 0)
        return false;

    DevicePayload* sc = (DevicePayload*) malloc(sizeof(DevicePayload));
    *sc = this->received_payloads.front();
    *payload = sc;

    this->received_payloads.pop();

    return true;
}

uint32_t SerialImpl::allocate_device_memory(uint32_t size_in_bytes)
{
    return this->device_memory->allocate(size_in_bytes);
}

void SerialImpl::write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data)
{
    this->device_memory->write(address, size_in_bytes, data);
}

char* SerialImpl::read_from_device_memory(uint32_t address, uint32_t size_in_bytes)
{
    return this->device_memory->read(address, size_in_bytes);
}

void SerialImpl::serial_read_process(uint8_t data)
{
    this->scratch.packet[this->scratch_ptr++] = data;

    if(this->scratch_ptr == 4)
        this->scratch_ptr_max = this->scratch.fields.num_bytes + 4;

    if(this->scratch_ptr == this->scratch_ptr_max) {
        this->received_payloads.push(this->scratch);

        this->scratch_ptr = 0;
        this->scratch_ptr_max = 8;
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
            this->log->log_info("Failed to open serial port '" + port_string + "'.");
            continue;
        }

        this->configure_serial_port(this->device_config["fpga"]["baud_rate"]);
        tcflush(this->port_fd, TCIOFLUSH);

        DevicePayload tx;
        tx.fields.id = 0;
        tx.fields.cmd = 1;
        tx.fields.sub_cmd = 0;
        tx.fields.num_bytes = 0;
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

                if(total_bytes_read == 6) {
                    this->log->log_info("Read 6 bytes from device.");
                    if(rx.fields.body_1 == 2 && rx.fields.body_2 == 1
                    ) {
                        this->log->log_info("Found device in serial port '" + port_string + "'.");
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
        this->log->log_error_and_exit("Could not find device over serial port.");
}