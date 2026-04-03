#include "matrix.h"
#include "device.h"
#include "device_memory.h"
#include "logging.h"
#include "symbols.h"

Device* presm_device = nullptr;
DeviceMemory* presm_device_memory = nullptr;

Logger drv_log;

void mInit()
{
    drv_log.init("driver");
    drv_log.log_info("mInit() called");
    
    presm_device = get_device();
    presm_device_memory = get_device_memory();

    char halt_symbol = HALT_SYMBOL;
    presm_device_memory->write(0, 1, &halt_symbol);
}