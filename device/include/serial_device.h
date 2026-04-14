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
    
    void device_initialize() override;

    virtual void serial_read_process(char data) = 0;

private:
    void find_device(std::string ident_str);
    bool open_serial_port(std::string port_name);
    void configure_serial_port(uint32_t baud_rate = 115200);

    void serial_port_write(char* data, uint32_t length);
    char serial_port_read_byte();

    std::queue<char> write_queue;
    std::queue<char> read_queue;

    std::string port;
    int port_fd;

    std::thread serial_port_listen_thread;
    std::thread serial_port_read_process_thread;
};

#endif