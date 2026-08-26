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

// void mAdd(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result)
// {
//     char add_cmd[4] = {
//         0x61, 0x64, 0x64, 0x69
//     };

//     std::cout << first.address << std::endl;

//     char add_cmd_param_1[4] = {
//         static_cast<char>(first.address & (255)),
//         static_cast<char>(first.address & (255 << 8)),
//         static_cast<char>(first.address & (255 << 16)),
//         static_cast<char>(first.address & (255 << 24))
//     };

//     char add_cmd_param_2[4] = {
//         static_cast<char>(second.address & (255)),
//         static_cast<char>(second.address & (255 << 8)),
//         static_cast<char>(second.address & (255 << 16)),
//         static_cast<char>(second.address & (255 << 24))
//     };

//     char add_cmd_param_3[4] = {
//         static_cast<char>(result.address & (255)),
//         static_cast<char>(result.address & (255 << 8)),
//         static_cast<char>(result.address & (255 << 16)),
//         static_cast<char>(result.address & (255 << 24))
//     };

//     presm_device->send_device_packet(4, add_cmd);
//     presm_device->send_device_packet(4, add_cmd_param_1);
//     presm_device->send_device_packet(4, add_cmd_param_2);
//     presm_device->send_device_packet(4, add_cmd_param_3);
// }

void mFree()
{
    delete presm_device;
}

// MIntDeviceMemory::MIntDeviceMemory(int source)
// {
//     this->size_in_bytes = 4;
//     this->address = presm_device->allocate_device_memory(this->size_in_bytes);

//     char data[4] = {
//         static_cast<char>(source & 255),
//         static_cast<char>(source & (255 << 8)),
//         static_cast<char>(source & (255 << 16)),
//         static_cast<char>(source & (255 << 24))
//     };
//     presm_device->write_to_device_memory(this->address, this->size_in_bytes, data);
// }

// int MIntDeviceMemory::getValue()
// {
//     char* data = presm_device->read_from_device_memory(this->address, this->size_in_bytes);

//     int rval = 0;
//     rval = rval | (data[0]) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

//     return rval;
// }