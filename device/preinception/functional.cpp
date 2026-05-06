#include "functional.h"

Device* get_device()
{
    return new Functional();
}

Functional::Functional()
{
    this->log.log_info("Functional 'Preinception' constructor called");
}