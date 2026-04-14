#include "matrix.h"
#include "device.h"
#include "logging.h"

Device* presm_device = nullptr;

Logger drv_log;

void mInit()
{
    drv_log.init("driver");
    drv_log.log_info("mInit() called");
    
    presm_device = get_device();
}

void mFree()
{
    delete presm_device;
}