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
    this->log->log_info("Received from device: " + std::to_string(data));
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

        uint8_t tx[4] = {1, 0, 0, 0};
        ssize_t bytes_written = write(this->port_fd, tx, 4);
        if (bytes_written != 4)
            this->log->log_error_and_exit("Failed initialize handshake over UART.");

        uint8_t rx[6];
        uint32_t total_bytes_read = 0;
        auto begin_time = std::chrono::high_resolution_clock::now();
        uint8_t rx_ptr = 0;
        while(true) {
            ssize_t bytes_read = read(this->port_fd, rx + rx_ptr, 1);

            if(bytes_read > 0) {
                rx_ptr += static_cast<uint8_t>(bytes_read);
                total_bytes_read += bytes_read;

                if(total_bytes_read == 6) {
                    this->log->log_info("Read 6 bytes from device.");
                    if(rx[4] == 2 && rx[5] == 1
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