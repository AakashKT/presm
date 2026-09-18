#ifndef HW_MODULE_H
#define HW_MODULE_H

#include "common.h"
#include "logging.h"
#include "hw_clock.h"

template <typename T>
struct HwMessage {
    std::shared_ptr<T> data;
    bool ack = true;
};

template <typename T>
class HwModule {
public:
    HwModule(uint32_t num_parallel_blocks, HwClock& clk);
    ~HwModule();

    void push_message(std::string module_name, std::shared_ptr<T> payload);
    
    bool get_message_ack(std::string module_name);
    
    virtual std::string module_name() = 0;
    virtual void execute(uint32_t block_idx) {};
    
protected:
    HwMessage<T> get_message(std::string module_name);

private:
    std::map<std::string, HwMessage<T>> messages;
    std::shared_mutex mtx_messages;
    
    std::vector<std::thread> execute_thread_list;
};

template <typename T>
HwModule<T>::HwModule(uint32_t num_parallel_blocks, HwClock& clk)
{
    for(auto i=0; i<num_parallel_blocks; i++)
        this->execute_thread_list.emplace_back(
            std::thread(
                [](HwModule *hw_module, uint32_t block_idx, HwClock* clk) {
                    while(true) {
                        clk->wait_for_tick();
                        hw_module->execute(block_idx);
                    }
                },
                this, i, &clk
            )
        );
}

template <typename T>
HwModule<T>::~HwModule()
{
    for(auto& t : this->execute_thread_list)
        t.detach();
}

template <typename T>
void HwModule<T>::push_message(std::string module_name, std::shared_ptr<T> payload)
{
    std::unique_lock<std::shared_mutex> lock(this->mtx_messages);

    this->messages[module_name].data = payload;
    this->messages[module_name].ack = false;
}

template <typename T>
bool HwModule<T>::get_message_ack(std::string module_name)
{
    std::unique_lock<std::shared_mutex> lock(this->mtx_messages);
    return this->messages[module_name].ack;
}

template <typename T>
HwMessage<T> HwModule<T>::get_message(std::string module_name)
{
    std::unique_lock<std::shared_mutex> lock(this->mtx_messages);

    HwMessage<T> rval;
    rval.data = std::make_shared<T>();

    auto it = this->messages.find(module_name);
    if(it != this->messages.end() && it->second.ack == false) {
        memcpy(rval.data.get(), it->second.data.get(), sizeof(T));
        rval.ack = it->second.ack;

        this->messages[module_name].data = nullptr;
        this->messages[module_name].ack = true;
    }

    return rval;
}

#endif

