#include "serial_device.h"

SerialDevice::SerialDevice()
    : Device()
{
    this->log.log_info("SerialDevice constructor called");

    this->find_device(this->device_config["fpga"]["identity_string"]);
    this->configure_serial_port(this->device_config["fpga"]["baud_rate"]);
}

SerialDevice::~SerialDevice()
{
    this->log.log_info("SerialDevice destructor called");

    this->serial_port_listen_thread.detach();
    this->serial_port_read_process_thread.detach();
    
    char reset_cmd[4] = {
        0x72,
        0x65,
        0x73,
        0x74
    };
    write(this->port_fd, reset_cmd, 4);
    close(this->port_fd);
}

void SerialDevice::find_device(std::string ident_str)
{
    char device_ident[5] = { '\0' };
    char handshake_cmd[4] = {
        0x69,
        0x64,
        0x65,
        0x6e
    };
    std::string port_string = "/dev/ttyUSB";

    for(uint32_t i=0; i<5; i++) {
        std::string current_port_string = port_string + std::to_string(i);
        bool port_opened = this->open_serial_port(current_port_string);
        
        if(port_opened) {
            this->log.log_info("Checking port '" + current_port_string + "'");

            tcflush(this->port_fd, TCIOFLUSH); 
            auto begin_time = std::chrono::high_resolution_clock::now();
            while(true) {
                auto bytes_read = read(this->port_fd, device_ident, 4);
                if(bytes_read > 0) {
                    this->log.log_info("Received '" + std::string(device_ident) + "' from device...");

                    if(device_ident[0] == ident_str[0] &&
                        device_ident[1] == ident_str[1] &&
                        device_ident[2] == ident_str[2] &&
                        device_ident[3] == ident_str[3]
                    ) {
                        this->log.log_info("Found device! Serial port '" + current_port_string + "' opened.");
                        write(this->port_fd, handshake_cmd, 4);
                        tcflush(this->port_fd, TCIOFLUSH); 

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

    this->log.log_error_and_exit("Could not find device over serial port");
}

void SerialDevice::device_initialize()
{
    this->serial_port_listen_thread = std::thread(
        [&](SerialDevice* current_device) {
            char data;
            
            while(true) {
                auto bytes_read = read(this->port_fd, &data, 1);
                if(bytes_read > 0)
                    current_device->read_queue.push(data);
            }
        },
        this
    );

    this->serial_port_read_process_thread = std::thread(
        [&](SerialDevice* current_device) {
            while(true) {
                if(current_device->read_queue.size() != 0) {
                    current_device->serial_read_process(current_device->read_queue.front());
                    current_device->read_queue.pop();
                }
            }
        },
        this
    );
}

bool SerialDevice::open_serial_port(std::string port_name)
{
    this->port_fd = open(port_name.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (this->port_fd < 0)
        return false;

    this->log.log_info("Serial port '" + port_name + "' opened.");
    return true;
}

void SerialDevice::configure_serial_port(uint32_t baud_rate)
{
    struct termios tty;
    if (tcgetattr(this->port_fd, &tty) != 0)
        this->log.log_error_and_exit("Error from tcgetattr: " + std::string(strerror(errno)));

    if(baud_rate == 115200) {
        cfsetospeed(&tty, B115200);
        cfsetispeed(&tty, B115200);
    }
    else {
        this->log.log_error_and_exit("No rule for baud rate " + std::to_string(baud_rate));
    }

    // tty.c_cflag |= (CLOCAL | CREAD); // Enable receiver, ignore control lines
    // tty.c_cflag &= ~CSIZE;
    // tty.c_cflag |= CS8;      // 8-bit characters
    // tty.c_cflag &= ~PARENB;  // No parity
    // tty.c_cflag &= ~CSTOPB;  // 1 stop bit

    // // Critical for non-blocking: Disable canonical mode
    // tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    
    // // Set timeout to 0 for immediate return
    // tty.c_cc[VMIN] = 0;
    // tty.c_cc[VTIME] = 0;

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit characters
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0; // no signaling chars, no echo, no
                     // canonical processing
    tty.c_oflag = 0; // no remapping, no delays
    tty.c_cc[VMIN] = 0; // read doesn't block
    tty.c_cc[VTIME] = 0; // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls,
                             // enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(this->port_fd, TCSANOW, &tty) != 0)
        this->log.log_error_and_exit("Error from tcsetattr: " + std::string(strerror(errno)));
}
