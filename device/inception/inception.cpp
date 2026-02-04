#include "inception.h"

Device* get_device()
{
    return new Inception();
}

Inception::Inception()
{
    this->log.log_info("Inception constructor called");
}