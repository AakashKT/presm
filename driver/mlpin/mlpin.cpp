#include "mlpin.h"
#include "device.h"
#include "logging.h"
#include "g_config_mlpin.h"

Device* device = nullptr;
Logger drv_log;

void mlpin_init()
{
    device = get_device();
    device->device_initialize();
    
    drv_log.init("driver");
    drv_log.log_info("Driver Init");
}