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
    
    virtual void serial_read_process(char data) = 0;
    virtual void device_find() = 0;

    void device_initialize() override;

    bool open_serial_port(std::string port_name);
    void configure_serial_port(uint32_t baud_rate = 115200);
    
    char read_buffer[256];
    uint32_t read_buffer_ptr_lo = 0;
    uint32_t read_buffer_ptr_hi = 0;
    
    int port_fd;

private:
    std::thread serial_port_listen_thread;
    std::thread serial_port_read_process_thread;
};

#endif