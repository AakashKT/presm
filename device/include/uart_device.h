#ifndef UART_DEVICE_H
#define UART_DEVICE_H

#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <unistd.h>

#include "device.h"

class UartDevice : public Device {
public:
    UartDevice();

private:
    void open_serial_port(std::string port_name);
    bool configure_serial_port(uint32_t baud_rate = 115200);

    void serial_port_write(char byte);
    void begin_serial_port_read();

    std::vector<char> write_queue;
    std::vector<char> read_queue;

    int port_fd;
};

#endif