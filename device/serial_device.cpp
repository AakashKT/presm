#include "serial_device.h"

SerialDevice::SerialDevice()
    : Device()
{
    this->log->log_info("SerialDevice constructor called");

    this->find_device(this->device_config["fpga"]["identity_string"]);
}

SerialDevice::~SerialDevice()
{
    this->log->log_info("SerialDevice destructor called");

    this->serial_port_listen_thread.detach();
    
    char reset_cmd[4] = {0x72, 0x65, 0x73, 0x74}; // 'rest'
    delete this->log;

    write(this->port_fd, reset_cmd, 4);
    close(this->port_fd);
}

void SerialDevice::find_device(std::string ident_str)
{
    char device_ident[5] = { '\0' };
    char handshake_cmd[4] = {0x69, 0x64, 0x65, 0x6e}; // 'iden'
    std::string port_string = "/dev/ttyUSB";

    for(uint32_t i=0; i<5; i++) {
        std::string current_port_string = port_string + std::to_string(i);
        bool port_opened = this->open_serial_port(current_port_string);
        
        if(port_opened) {
            this->configure_serial_port(this->device_config["fpga"]["baud_rate"]);
            this->log->log_info("Checking port '" + current_port_string + "'");

            tcflush(this->port_fd, TCIOFLUSH);

            auto begin_time = std::chrono::high_resolution_clock::now();
            while(true) {
                auto bytes_read = read(this->port_fd, device_ident, 4);
                if(bytes_read > 0) {
                    this->log->log_info("Received '" + std::string(device_ident) + "' from device...");

                    if(device_ident[0] == ident_str[0] &&
                        device_ident[1] == ident_str[1] &&
                        device_ident[2] == ident_str[2] &&
                        device_ident[3] == ident_str[3]
                    ) {
                        this->log->log_info("Found device! Serial port '" + current_port_string + "' opened.");
                        write(this->port_fd, handshake_cmd, 4);

                        return;
                    }
                }

                auto time_diff = std::chrono::high_resolution_clock::now() - begin_time;
                if(std::chrono::duration_cast<std::chrono::microseconds>(time_diff).count() >= 2e6)
                    break;
            }
            
            tcflush(this->port_fd, TCIOFLUSH);
            close(this->port_fd);
        }
    }

    this->log->log_error_and_exit("Could not find device over serial port");
}

void SerialDevice::device_initialize()
{
    this->serial_port_listen_thread = std::thread(
        [&](SerialDevice* current_device) {
            char data[256];

            uint32_t bytes_read = 0, total_bytes_read = 0;

            while(true) {
                bytes_read = read(this->port_fd, data + total_bytes_read, 12);

                if(bytes_read > 0) {
                    total_bytes_read += bytes_read;

                    if(total_bytes_read == 12) {
                        for(uint32_t i=0; i<12; i++) {
                            current_device->read_buffer[current_device->read_buffer_ptr_hi % 256] = data[i];
                            current_device->read_buffer_ptr_hi++;
                        }
                        
                        total_bytes_read = 0;
                    }
                }
            }
        },
        this
    );

    this->serial_port_read_process_thread = std::thread(
        [&](SerialDevice* current_device) {
            while(true) {
                if(current_device->read_buffer_ptr_hi != current_device->read_buffer_ptr_lo) {
                    current_device->serial_read_process(current_device->read_buffer[current_device->read_buffer_ptr_lo % 256]);
                    current_device->read_buffer_ptr_lo++;
                }
            }
        },
        this
    );
}

void SerialDevice::send_device_packet(uint32_t size_in_bytes, char* packet)
{
    write(this->port_fd, packet, size_in_bytes);
    
    std::stringstream ss;
    for(uint32_t i=0; i<size_in_bytes; i++)
        ss << std::hex << (int)packet[i];
}

void SerialDevice::serial_port_write_block(uint32_t size_in_bytes, char* data)
{
    while(true) {
        auto bytes_written = write(this->port_fd, data, size_in_bytes);
        if(bytes_written == size_in_bytes)
            break;
    }
}

bool SerialDevice::open_serial_port(std::string port_name)
{
    this->port_fd = open(port_name.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (this->port_fd < 0)
        return false;

    this->log->log_info("Serial port '" + port_name + "' opened.");
    return true;
}

void SerialDevice::configure_serial_port(uint32_t baud_rate)
{
    struct termios tty;
    if (tcgetattr(this->port_fd, &tty) != 0)
        this->log->log_error_and_exit("Error from tcgetattr: " + std::string(strerror(errno)));

    if(baud_rate == 115200) {
        cfsetospeed(&tty, B115200);
        cfsetispeed(&tty, B115200);
    }
    else {
        this->log->log_error_and_exit("No rule for baud rate " + std::to_string(baud_rate));
    }

    tty.c_cflag |= (CLOCAL | CREAD);    // Ignore modem lines, enable receiver
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;                 // 8-bit characters
    tty.c_cflag &= ~PARENB;             // No parity bit
    tty.c_cflag &= ~CSTOPB;             // Only one stop bit
    tty.c_cflag &= ~CRTSCTS;            // Disable hardware flow control
    tty.c_cflag &= ~ICRNL;
    tty.c_cflag &= ~IXON;

    tty.c_oflag &= ~OPOST;

    // Critical for non-blocking: Disable canonical mode
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_lflag &= ~ECHOK;
    tty.c_lflag &= ~ECHOKE;
    tty.c_lflag &= ~ECHOPRT;
    tty.c_lflag &= ~ECHOCTL;
    
    // Set timeout to 0 for immediate return
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 5;

    if (tcsetattr(this->port_fd, TCSANOW, &tty) != 0)
        this->log->log_error_and_exit("Error from tcsetattr: " + std::string(strerror(errno)));
}
