#include "serial.h"

Device* get_device()
{
    return new Serial();
}

