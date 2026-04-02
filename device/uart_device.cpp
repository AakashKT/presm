#include "uart_device.h"

UartDevice::UartDevice()
    : Device()
{
    this->log.log_info("UartDevice constructor called");

    this->open_serial_port(this->device_config["fpga"]["linux_device_path"]);
    this->configure_serial_port(this->device_config["fpga"]["baud_rate"]);
}

void UartDevice::serial_port_listen()
{

}

void UartDevice::open_serial_port(std::string port_name)
{
    this->port_fd = open(port_name.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (this->port_fd < 0)
        this->log.log_error_and_exit("Could not open serial port " + port_name + ", Error: " + std::string(strerror(errno)));

    this->log.log_info("Serial port '" + port_name + "' opened.");
}

void UartDevice::configure_serial_port(uint32_t baud_rate)
{
    struct termios tty;
    if (tcgetattr(this->port_fd, &tty) != 0)
        this->log.log_error_and_exit("Error from tcgetattr: " + std::string(strerror(errno)));

    cfsetospeed(&tty, baud_rate);
    cfsetispeed(&tty, baud_rate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit characters
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0; // no signaling chars, no echo, no
                     // canonical processing
    tty.c_oflag = 0; // no remapping, no delays
    tty.c_cc[VMIN] = 0; // read doesn't block
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls,
                             // enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(this->port_fd, TCSANOW, &tty) != 0)
        this->log.log_error_and_exit("Error from tcsetattr: " + std::string(strerror(errno)));
}
