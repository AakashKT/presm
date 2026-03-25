#include "matrix.h"
#include "device.h"

Device* presm_device = nullptr;

void mInit()
{
    presm_device = get_device();
    std::cout << "mInit() called" << std::endl;
}