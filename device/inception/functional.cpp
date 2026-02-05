#include "functional.h"

Device* get_device()
{
    return new Functional();
}

Functional::Functional()
    : Device()
{
    this->log.log_info("Functional 'Inception' constructor called");
}