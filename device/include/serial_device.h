#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "device.h"

class SerialDevice : public Device {
public:
    SerialDevice();
    ~SerialDevice();
    
    void serial_port_write_block(uint32_t size_in_bytes, char* data);
    void send_device_packet(uint32_t size_in_bytes, char* packet) override;
    void device_initialize() override;
    
    virtual void serial_read_process(uint8_t data) = 0;
    
    uint8_t read_buffer[256];
    uint32_t read_buffer_ptr_lo = 0;
    uint32_t read_buffer_ptr_hi = 0;
    
private:
    bool open_serial_port(std::string port_name);
    void configure_serial_port(uint32_t baud_rate = 115200);

    int port_fd;

    uint8_t start_byte, identity_byte, reset_byte;

    std::thread serial_port_listen_thread;
    std::thread serial_port_read_process_thread;
};

#endif