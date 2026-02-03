#include "inception.h"

Device* get_device()
{
    return new Inception();
}

Inception::Inception()
{
    this->device_log.log_info("Inception constructor called");
}