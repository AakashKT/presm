#include "uart_device.h"

UartDevice::UartDevice()
    : Device()
{
    this->device_log.log_info("UartDevice constructor called");
}

void UartDevice::open_serial_port(std::string port_name)
{
    this->port_fd = open(port_name.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (this->port_fd < 0) {
        
    }
}

bool UartDevice::configure_serial_port(uint32_t baud_rate)
{
    return false;
}
