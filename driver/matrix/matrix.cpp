#include "matrix.h"
#include "device.h"
#include "logging.h"

union DevicePacket {

    struct Fields {
        uint32_t id : 8;
        uint32_t cmd : 8;
        uint32_t sub_cmd : 8;
        uint32_t num_bytes : 8;
        uint32_t body_1 : 8;
        uint32_t body_2 : 8;
        uint32_t body_3 : 8;
        uint32_t body_4 : 8;
    } fields;

    uint8_t packet[8];
};

Device* presm_device = nullptr;
Logger drv_log;

std::string intToHex(const int& num) 
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << num;

    return "0x" + ss.str();
}

void mInit()
{
    drv_log.init("driver");
    drv_log.log_info("Driver init called");
    
    presm_device = get_device();
}

void mAdd(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result)
{
    DevicePacket pkt;
    pkt.fields.id = (uint8_t) 1;
    pkt.fields.cmd = (uint8_t) 0;
    pkt.fields.sub_cmd = (uint8_t) 0;
    pkt.fields.num_bytes = (uint8_t) 0;

    presm_device->send_device_packet((void*)pkt.packet, 32);
    usleep(1000000);
}

void mFree()
{
    drv_log.log_info("Driver free called");
    delete presm_device;
}

MIntDeviceMemory::MIntDeviceMemory(int source)
{
    this->size_in_bytes = 4;
    this->address = presm_device->allocate_device_memory(this->size_in_bytes);

    char data[4] = {
        static_cast<char>(source & 255),
        static_cast<char>(source & (255 << 8)),
        static_cast<char>(source & (255 << 16)),
        static_cast<char>(source & (255 << 24))
    };
    presm_device->write_to_device_memory(this->address, this->size_in_bytes, data);

    std::string hex_addr = intToHex(this->address);
    drv_log.log_info("Allocated int on device at " + hex_addr);
}

int MIntDeviceMemory::getValue()
{
    std::string hex_addr = intToHex(this->address);
    drv_log.log_info("Reading int from device at " + hex_addr);

    char* data = presm_device->read_from_device_memory(this->address, this->size_in_bytes);

    int rval = 0;
    rval = rval | (data[0]) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

    return rval;
}