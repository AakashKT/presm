#include "device.h"

Device::Device()
{
    this->log = new Logger();
    this->log->init("device");

    this->log->log_info("[Device] Constructor called");
}