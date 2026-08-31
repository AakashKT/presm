#include "hw_module.h"

HwModule::HwModule()
{
    this->execute_thread = std::thread(
        [](HwModule *hw_module) {
            while(true) {
                hw_module->execute();
                // usleep(1000);
            }
        },
        this
    );
}

HwModule::~HwModule()
{
    this->execute_thread.detach();
}

void HwModule::push_message(void* payload)
{
    std::unique_lock<std::shared_mutex> lock(this->mtx_messages);
    this->messges.push_back(payload);
}

void* HwModule::get_message()
{
    std::shared_lock<std::shared_mutex> lock(this->mtx_messages);
    
    void* rval;
    if(this->messges.empty()) {
        rval = nullptr;
    }
    else {
        rval = this->messges.front();
        this->messges.pop_front();
    }

    return rval;
}

void HwModule::connect_to(HwModule& module)
{
    this->connections.push_back(&module);
}