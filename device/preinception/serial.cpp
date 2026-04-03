#include "serial.h"

Device* get_device()
{
    return new SerialImpl();
}

SerialImpl::SerialImpl()
{
    this->initialize();
}

void SerialImpl::handle_incoming_byte(char data)
{
    char temp[2] = {data, '\0'};
    this->log.log_info(std::string(temp));
}