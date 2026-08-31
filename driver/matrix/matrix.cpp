#include "matrix.h"
#include "device.h"
#include "logging.h"

#include "defs.h"

Device* presm_device = nullptr;
Logger drv_log;

uint8_t global_command_id = 0;

std::list<DevicePayload> recorded_commands;

ThreadSafeList<DevicePayload> command_buffer;
ThreadSafeList<std::pair<DevicePayload, bool>> command_status_readback;

std::thread command_process_thread;
std::thread device_payload_receive_thread;

std::string intToHex(const int& num) 
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << num;

    return "0x" + ss.str();
}

void cmdSync(char id)
{
    bool finished = false;
    while(true) {
        if(!command_status_readback.empty()) {
            command_status_readback.for_each(
                [&](std::pair<DevicePayload, bool>& item) {
                    if(item.second == false && item.first.fields.id == id && item.first.fields.type == 1) {
                        finished = true;
                        item.second = true;
                    }
                }
            );
        }

        if(finished) {
            break;
        }
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
                    command_status_readback.push_back(std::pair(*scratch, false));
                }
            }
        }
    );
}

MCommandInfo mAdd(MUIntDeviceMemory& first, MUIntDeviceMemory& second, MUIntDeviceMemory& result)
{
    // [NOTE:]
    // This is strange - on and after ID of 13, the FPGA (TangNano20k) behaves weird, thorwing the sync off
    // No idea why, testbenches with ID=13 and above work fine - maybe this particular FPGA piece has a hardware error?
    global_command_id = global_command_id % 10;

    DevicePayload add_op_1;
    add_op_1.fields.id = static_cast<char>(global_command_id++);
    add_op_1.fields.type = static_cast<char>(0);
    add_op_1.fields.cmd = static_cast<char>(2);
    add_op_1.fields.sub_cmd = static_cast<char>(0);
    add_op_1.fields32.body = first.address;
    
    DevicePayload add_op_2;
    add_op_2.fields.id = static_cast<char>(global_command_id++);
    add_op_2.fields.type = static_cast<char>(0);
    add_op_2.fields.cmd = static_cast<char>(2);
    add_op_2.fields.sub_cmd = static_cast<char>(1);
    add_op_2.fields32.body = second.address;

    DevicePayload add_op_3;
    add_op_3.fields.id = static_cast<char>(global_command_id++);
    add_op_3.fields.type = static_cast<char>(0);
    add_op_3.fields.cmd = static_cast<char>(2);
    add_op_3.fields.sub_cmd = static_cast<char>(2);
    add_op_3.fields32.body = result.address;

    recorded_commands.push_back(add_op_1);
    recorded_commands.push_back(add_op_2);
    recorded_commands.push_back(add_op_3);
    
    return MCommandInfo(add_op_3.fields.id);
}

MCommandInfo mAdd(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result)
{
    // [NOTE:]
    // This is strange - on and after ID of 13, the FPGA (TangNano20k) behaves weird, thorwing the sync off
    // No idea why, testbenches with ID=13 and above work fine - maybe this particular FPGA piece has a hardware error?
    global_command_id = global_command_id % 10;

    DevicePayload add_op_1;
    add_op_1.fields.id = static_cast<char>(global_command_id++);
    add_op_1.fields.type = static_cast<char>(0);
    add_op_1.fields.cmd = static_cast<char>(2);
    add_op_1.fields.sub_cmd = static_cast<char>(0);
    add_op_1.fields32.body = first.address;
    
    DevicePayload add_op_2;
    add_op_2.fields.id = static_cast<char>(global_command_id++);
    add_op_2.fields.type = static_cast<char>(0);
    add_op_2.fields.cmd = static_cast<char>(2);
    add_op_2.fields.sub_cmd = static_cast<char>(1);
    add_op_2.fields32.body = second.address;

    DevicePayload add_op_3;
    add_op_3.fields.id = static_cast<char>(global_command_id++);
    add_op_3.fields.type = static_cast<char>(0);
    add_op_3.fields.cmd = static_cast<char>(2);
    add_op_3.fields.sub_cmd = static_cast<char>(2);
    add_op_3.fields32.body = result.address;

    recorded_commands.push_back(add_op_1);
    recorded_commands.push_back(add_op_2);
    recorded_commands.push_back(add_op_3);
    
    return MCommandInfo(add_op_3.fields.id);
}

void mSync(MCommandInfo& info)
{
    while(true) {
        if(recorded_commands.empty())
            break;

        auto payload = recorded_commands.front();

        command_buffer.push_back(payload);
        cmdSync(payload.fields.id);

        recorded_commands.pop_front();
    }

    drv_log.log_info("mSync finished for ID: " + std::to_string(info.id));
}

void mFree()
{
    drv_log.log_info("Driver free called");

    command_process_thread.detach();
    device_payload_receive_thread.detach();
}

MUIntDeviceMemory::MUIntDeviceMemory(uint32_t source)
{
    this->size_in_bytes = 4;
    this->address = presm_device->allocate_device_memory(this->size_in_bytes);

    char data[4] = { source & 255, (source & (255 << 8)) >> 8, (source & (255 << 16)) >> 16, (source & (255 << 24)) >> 24 };
    presm_device->write_to_device_memory(this->address, this->size_in_bytes, data);

    drv_log.log_info("Allocated int on device at " + intToHex(this->address));
}

uint32_t MUIntDeviceMemory::getValue()
{
    drv_log.log_info("Reading UInt from device at " + intToHex(this->address));

    char* data = presm_device->read_from_device_memory(this->address, this->size_in_bytes);

    uint32_t rval = 0;
    rval = rval | (255 & data[0]) | ((255 & data[1]) << 8) | ((255 & data[2]) << 16) | ((255 & data[3]) << 24);

    return rval;
}

MIntDeviceMemory::MIntDeviceMemory(int32_t source)
{
    this->size_in_bytes = 4;
    this->address = presm_device->allocate_device_memory(this->size_in_bytes);

    char data[4] = { source & 255, (source & (255 << 8)) >> 8, (source & (255 << 16)) >> 16, (source & (255 << 24)) >> 24 };
    presm_device->write_to_device_memory(this->address, this->size_in_bytes, data);

    drv_log.log_info("Allocated int on device at " + intToHex(this->address));
}

int32_t MIntDeviceMemory::getValue()
{
    drv_log.log_info("Reading UInt from device at " + intToHex(this->address));

    char* data = presm_device->read_from_device_memory(this->address, this->size_in_bytes);

    int32_t rval = 0;
    rval = rval | (255 & data[0]) | ((255 & data[1]) << 8) | ((255 & data[2]) << 16) | ((255 & data[3]) << 24);

    return rval;
}