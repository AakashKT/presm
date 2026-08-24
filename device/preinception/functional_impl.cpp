#include "functional_impl.h"

Device* get_device()
{
    return new FunctionalImpl();
}

FunctionalImpl::FunctionalImpl()
{
    this->log->log_info("FunctionalImpl 'Preinception' constructor called");
}