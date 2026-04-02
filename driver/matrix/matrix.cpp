#include "matrix.h"
#include "device.h"
#include "device_memory.h"
#include "logging.h"

Device* presm_device = nullptr;
DeviceMemory* presm_device_memory = nullptr;

Logger drv_log;

void mInit()
{
    drv_log.init("driver");
    drv_log.log_info("mInit() called");
    
    presm_device = get_device();
    presm_device_memory = get_device_memory();

    
}