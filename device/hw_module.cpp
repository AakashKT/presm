#include "hw_module.h"

HwModule::HwModule(uint32_t num_parallel_blocks)
{
    for(auto i=0; i<num_parallel_blocks; i++)
        this->execute_thread_list.push_back(
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

void HwModule::push_message(std::string module_name, void* payload)
{
    std::unique_lock<std::shared_mutex> lock(this->mtx_messages);

    auto it = this->messges.find(module_name);
    if(it != this->messges.end() && it->second != nullptr)
        free(this->messges[module_name]);

    this->messges[module_name] = payload;
}

void* HwModule::get_message(std::string module_name)
{
    std::unique_lock<std::shared_mutex> lock(this->mtx_messages);

    void* rval;

    auto it = this->messges.find(module_name);
    if(it == this->messges.end())
        rval = nullptr;
    else
        rval = it->second;
    
    this->messges[module_name] = nullptr;

    return rval;
}
