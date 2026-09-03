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

void drv_preproc()
{
    // [NOTE:]
    // This is strange - on and above ID=13, the FPGA (TangNano20k) behaves weird, thorwing the sync off
    // No idea why, testbenches with ID=13 and above work fine - maybe this particular FPGA piece has a hardware error?
    global_command_id = global_command_id % 10;
}

void cmdSync(uint32_t id)
{
    bool finished = false;
    while(true) {
        if(!command_status_readback.empty()) {
            command_status_readback.for_each(
                [&](std::pair<DevicePayload, bool>& item) {
                    if(item.second == false && item.first.id() == id 
                        && item.first.type() == static_cast<uint32_t>(TYPE::RESPONSE)) {
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
    presm_device->device_initialize();

    command_process_thread = std::thread(
        [&]() {
            while(true) {
                auto payload_opt = command_buffer.pop_front();

                if(payload_opt != std::nullopt) {
                    DevicePayload payload = *payload_opt;
                    drv_log.log_info("Sending packet with ID: " + std::to_string(payload.id()));

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
                    drv_log.log_info("Received packet with ID: " + std::to_string(scratch->id()));
                    command_status_readback.push_back(std::pair(*scratch, false));
                }
            }
        }
    );
}

void mAdd(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result)
{
    drv_preproc();

    DevicePayload add_op_1;
    add_op_1.id(global_command_id++);
    add_op_1.type((uint32_t)TYPE::REQUEST);
    add_op_1.cmd((uint32_t)CMD::ADD);
    add_op_1.sub_cmd((uint32_t)ADD::OP_1);
    add_op_1.fields32.body = first.address;
    
    DevicePayload add_op_2;
    add_op_2.id(global_command_id++);
    add_op_2.type((uint32_t)TYPE::REQUEST);
    add_op_2.cmd((uint32_t)CMD::ADD);
    add_op_2.sub_cmd((uint32_t)ADD::OP_2);
    add_op_2.fields32.body = second.address;

    DevicePayload add_op_3;
    add_op_3.id(global_command_id++);
    add_op_3.type((uint32_t)TYPE::REQUEST);
    add_op_3.cmd((uint32_t)CMD::ADD);
    add_op_3.sub_cmd((uint32_t)ADD::OP_3);
    add_op_3.fields32.body = result.address;

    recorded_commands.push_back(add_op_1);
    recorded_commands.push_back(add_op_2);
    recorded_commands.push_back(add_op_3);
    
    return;
}

void mMulPow2(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result)
{
    drv_preproc();

    // Power cannot be negative
    if(second.getValue() < 0)
        drv_log.log_error_and_exit("Seond operand (power of 2) cannot be negative.");

    DevicePayload mul_op_1;
    mul_op_1.id(global_command_id++);
    mul_op_1.type((uint32_t)TYPE::REQUEST);
    mul_op_1.cmd((uint32_t)CMD::MULP2);
    mul_op_1.sub_cmd((uint32_t)MULP2::OP_1);
    mul_op_1.fields32.body = first.address;

    DevicePayload mul_op_2;
    mul_op_2.id(global_command_id++);
    mul_op_2.type((uint32_t)TYPE::REQUEST);
    mul_op_2.cmd((uint32_t)CMD::MULP2);
    mul_op_2.sub_cmd((uint32_t)MULP2::OP_2);
    mul_op_2.fields32.body = second.address;

    DevicePayload mul_op_3;
    mul_op_3.id(global_command_id++);
    mul_op_3.type((uint32_t)TYPE::REQUEST);
    mul_op_3.cmd((uint32_t)CMD::MULP2);
    mul_op_3.sub_cmd((uint32_t)MULP2::OP_3);
    mul_op_3.fields32.body = result.address;

    recorded_commands.push_back(mul_op_1);
    recorded_commands.push_back(mul_op_2);
    recorded_commands.push_back(mul_op_3);

    return;
}

void mDivPow2(MIntDeviceMemory& first, MIntDeviceMemory& second, MIntDeviceMemory& result)
{
    drv_preproc();

    // Power cannot be negative
    if(second.getValue() < 0)
        drv_log.log_error_and_exit("Seond operand (power of 2) cannot be negative.");

    DevicePayload divp2_op_1;
    divp2_op_1.id(global_command_id++);
    divp2_op_1.type((uint32_t)TYPE::REQUEST);
    divp2_op_1.cmd((uint32_t)CMD::DIVP2);
    divp2_op_1.sub_cmd((uint32_t)DIVP2::OP_1);
    divp2_op_1.fields32.body = first.address;

    DevicePayload divp2_op_2;
    divp2_op_2.id(global_command_id++);
    divp2_op_2.type((uint32_t)TYPE::REQUEST);
    divp2_op_2.cmd((uint32_t)CMD::DIVP2);
    divp2_op_2.sub_cmd((uint32_t)DIVP2::OP_2);
    divp2_op_2.fields32.body = second.address;

    DevicePayload divp2_op_3;
    divp2_op_3.id(global_command_id++);
    divp2_op_3.type((uint32_t)TYPE::REQUEST);
    divp2_op_3.cmd((uint32_t)CMD::DIVP2);
    divp2_op_3.sub_cmd((uint32_t)DIVP2::OP_3);
    divp2_op_3.fields32.body = result.address;

    recorded_commands.push_back(divp2_op_1);
    recorded_commands.push_back(divp2_op_2);
    recorded_commands.push_back(divp2_op_3);

    return;
}

void mSync()
{
    while(true) {
        if(recorded_commands.empty())
            break;

        auto payload = recorded_commands.front();

        command_buffer.push_back(payload);
        cmdSync(payload.id());

        recorded_commands.pop_front();
    }

    drv_log.log_info("mSync finished");
}

void mFree()
{
    drv_log.log_info("Driver free called");

    command_process_thread.detach();
    device_payload_receive_thread.detach();
}

MIntDeviceMemory::MIntDeviceMemory(int32_t source)
{
    this->size_in_bytes = 4;
    this->address = presm_device->allocate_device_memory(this->size_in_bytes);

    char data[4] = { source & 255, (source & (255 << 8)) >> 8, (source & (255 << 16)) >> 16, (source & (255 << 24)) >> 24 };
    presm_device->write_to_device_memory(this->address, this->size_in_bytes, data);

    drv_log.log_info("Allocated int on device at 0x" + intToHex(this->address));
}

int32_t MIntDeviceMemory::getValue()
{
    drv_log.log_info("Reading Int from device at 0x" + intToHex(this->address));

    char* data = presm_device->read_from_device_memory(this->address, this->size_in_bytes);

    int32_t rval = 0;
    rval = rval | (255 & data[0]) | ((255 & data[1]) << 8) | ((255 & data[2]) << 16) | ((255 & data[3]) << 24);

    return rval;
}

void MIntDeviceMemory::setValue(int32_t new_val)
{
    drv_log.log_info("Overwriting Int at 0x" + intToHex(this->address));

    char data[4] = { new_val & 255, (new_val & (255 << 8)) >> 8, (new_val & (255 << 16)) >> 16, (new_val & (255 << 24)) >> 24 };
    presm_device->write_to_device_memory(this->address, this->size_in_bytes, data);
}