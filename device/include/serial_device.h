#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <unistd.h>

#include "device.h"

class SerialDevice : public Device {
public:
    SerialDevice();
    
    void initialize();
    virtual void handle_incoming_byte(char data) = 0;

private:
    void open_serial_port(std::string port_name);
    void configure_serial_port(uint32_t baud_rate = 115200);

    void serial_port_write_byte(char byte);
    char serial_port_read_byte();

    std::vector<char> write_queue;
    std::vector<char> read_queue;

    int port_fd;

    std::thread serial_port_listen_thread;
};

#endif