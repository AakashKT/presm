#include "matrix.h"
#include "device.h"
#include "logging.h"

#include "defs.h"

Device* presm_device = nullptr;
Logger drv_log;

uint8_t global_command_id = 3;

std::queue<DevicePayload> command_buffer;
std::list<DevicePayload> command_status_readback;

std::thread command_process_thread;
std::thread device_payload_receive_thread;

std::string intToHex(const int& num) 
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << num;

    return "0x" + ss.str();
}

void cmdSync(uint8_t id)
{
    bool finished = false;
    while(true) {
        for(auto it=command_status_readback.begin(); it != command_status_readback.end(); it++) {
            if((*it).fields.id == id) {
                finished = true;
                break;
            }
        }

        if(finished)
            break;
    }
}

void mInit()
{
    drv_log.init("driver");
    drv_log.log_info("Driver init called");
    
    presm_device = get_device();

    command_process_thread = std::thread(
        [&]() {
            while(true) {
                if(command_buffer.size() != 0) {
                    DevicePayload& payload = command_buffer.front();
                    drv_log.log_info("Sending packet with ID: " + std::to_string(payload.fields.id));

                    presm_device->send_device_payload(&payload);
                    cmdSync(payload.fields.id);

                    command_buffer.pop();
                }
            }
        }
    );

    device_payload_receive_thread = std::thread(
        [&]() {
            DevicePayload* scratch;
            while(true) {
                if(presm_device->receive_device_payload((void**)&scratch)) {
                    drv_log.log_info("Received packet with ID: " + std::to_string(scratch->fields.id));
                    command_status_readback.push_back(*scratch);
                }
            }
        }
    );
}

MCommandInfo mAdd(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result)
{
    DevicePayload payload;
    payload.fields.id = (uint8_t) global_command_id++;
    payload.fields.cmd = (uint8_t) 1;
    payload.fields.sub_cmd = (uint8_t) 0;
    payload.fields.num_bytes = (uint8_t) 0;

    command_buffer.push(payload);
    
    return MCommandInfo(payload.fields.id);
}

void mSync(MCommandInfo& info)
{
    cmdSync(info.id);

    drv_log.log_info("mSync finished for ID: " + std::to_string(info.id));
}

void mFree()
{
    drv_log.log_info("Driver free called");

    command_process_thread.detach();
    device_payload_receive_thread.detach();
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