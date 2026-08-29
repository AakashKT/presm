#include "matrix.h"
#include "device.h"
#include "logging.h"

#include "defs.h"

Device* presm_device = nullptr;
Logger drv_log;

uint8_t global_command_id = 0;

ThreadSafeList<DevicePayload> command_buffer;
ThreadSafeList<DevicePayload> command_status_readback;

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
        if(!command_status_readback.empty()) {
            command_status_readback.for_each(
                [&](DevicePayload item) {
                    if(item.fields.id == id)
                        finished = true;
                }
            );
        }

        if(finished)
            break;
        else
            usleep(10);
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
                auto payload_opt = command_buffer.pop_front();

                if(payload_opt != std::nullopt) {
                    DevicePayload payload = *payload_opt;
                    drv_log.log_info("Sending packet with ID: " + std::to_string(payload.fields.id));

                    presm_device->send_device_payload(&payload);
                    cmdSync(payload.fields.id);
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
    DevicePayload add_op_1;
    add_op_1.fields.id = (uint8_t) global_command_id++;
    add_op_1.fields.type = (uint8_t) 0;
    add_op_1.fields.cmd = (uint8_t) 2;
    add_op_1.fields.sub_cmd = (uint8_t) 0;
    add_op_1.fields32.body = first.address;
    
    DevicePayload add_op_2;
    add_op_2.fields.id = (uint8_t) global_command_id++;
    add_op_2.fields.type = (uint8_t) 0;
    add_op_2.fields.cmd = (uint8_t) 2;
    add_op_2.fields.sub_cmd = (uint8_t) 0;
    add_op_2.fields32.body = second.address;

    // DevicePayload add_op_3;
    // add_op_3.fields.id = (uint8_t) global_command_id++;
    // add_op_3.fields.cmd = (uint8_t) 2;
    // add_op_3.fields.sub_cmd = (uint8_t) 2;
    // add_op_3.fields.num_bytes = (uint8_t) 4;
    // add_op_3.fields32.body = result.address;

    command_buffer.push_back(add_op_1);
    // command_buffer.push_back(add_op_2);
    // command_buffer.push(add_op_3);
    
    return MCommandInfo(add_op_1.fields.id);
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

    uint8_t data[4] = { source & 255, source & (255 << 8), source & (255 << 16), source & (255 << 24) };
    presm_device->write_to_device_memory(this->address, this->size_in_bytes, data);

    drv_log.log_info("Allocated int on device at " + intToHex(this->address));
}

int MIntDeviceMemory::getValue()
{
    drv_log.log_info("Reading int from device at " + intToHex(this->address));

    uint8_t* data = presm_device->read_from_device_memory(this->address, this->size_in_bytes);

    int rval = 0;
    rval = rval | (data[0]) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

    return rval;
}