#include "hw_module.h"

HwModule::HwModule(uint32_t num_parallel_blocks)
{
    for(auto i=0; i<num_parallel_blocks; i++)
        this->execute_thread_list.emplace_back(
            std::thread(
                [](HwModule *hw_module, uint32_t block_idx) {
                    while(true) {
                        hw_module->execute(block_idx);
                    }
                },
                this, i
            )
        );
}

HwModule::~HwModule()
{
    for(auto& t : this->execute_thread_list)
        t.detach();
}

void HwModule::push_message(std::string module_name, std::shared_ptr<void> payload)
{
    std::unique_lock<std::shared_mutex> lock(this->mtx_messages);

    this->messages[module_name].data = payload;
    this->messages[module_name].ack = false;
}

bool HwModule::get_message_ack(std::string module_name)
{
    std::unique_lock<std::shared_mutex> lock(this->mtx_messages);
    return this->messages[module_name].ack;
}

HwMessage HwModule::get_message(std::string module_name)
{
    std::unique_lock<std::shared_mutex> lock(this->mtx_messages);

    HwMessage rval;

    auto it = this->messages.find(module_name);
    if(it != this->messages.end() && it->second.ack == false) {
        rval.data = it->second.data;
        rval.ack = it->second.ack;

        this->messages[module_name].data = nullptr;
        this->messages[module_name].ack = true;
    }

    return rval;
}
