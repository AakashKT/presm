#include "serial_device.h"

SerialDevice::SerialDevice()
    : Device()
{
    this->log->log_info("[SerialDevice] Constructor called");
}

SerialDevice::~SerialDevice()
{
    this->log->log_info("[SerialDevice] Destructor called");

    this->serial_port_listen_thread.detach();
    this->serial_port_read_process_thread.detach();

    delete this->log;

    usleep(1000);
    tcflush(this->port_fd, TCIOFLUSH);
    usleep(1000);
    close(this->port_fd);
}

void SerialDevice::device_initialize()
{
    this->device_find();

    this->serial_port_listen_thread = std::thread(
        [&](SerialDevice* current_device) {
            while(true) {
                char data = 0;
                uint32_t bytes_read = read(current_device->port_fd, &data, 1);

                if(bytes_read > 0) {                    
                    current_device->read_buffer[current_device->read_buffer_ptr_hi % 256] = data;
                    current_device->read_buffer_ptr_hi++;
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

bool SerialDevice::open_serial_port(std::string port_name)
{
    this->port_fd = open(port_name.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (this->port_fd < 0)
        return false;

    this->log->log_info("[SerialDevice] Serial port '" + port_name + "' opened.");
    return true;
}

void SerialDevice::configure_serial_port(uint32_t baud_rate)
{
    struct termios tty;
    if (tcgetattr(this->port_fd, &tty) != 0)
        this->log->log_error_and_exit("[SerialDevice] Error from tcgetattr: " + std::string(strerror(errno)));

    if(baud_rate == 115200) {
        cfsetospeed(&tty, B115200);
        cfsetispeed(&tty, B115200);
    }
    else if(baud_rate == 9600) {
        cfsetospeed(&tty, B9600);
        cfsetispeed(&tty, B9600);
    }
    else {
        this->log->log_error_and_exit("[SerialDevice] No rule for baud rate " + std::to_string(baud_rate));
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
        this->log->log_error_and_exit("[SerialDevice] Error from tcsetattr: " + std::string(strerror(errno)));
    
    tcflush(this->port_fd, TCIOFLUSH);
}
