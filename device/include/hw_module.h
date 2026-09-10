#ifndef HW_MODULE_H
#define HW_MODULE_H

#include "common.h"
#include "logging.h"

struct HwMessage {
    std::shared_ptr<void> data;
    bool ack = false;
};

class HwModule {
public:
    HwModule(uint32_t num_parallel_blocks);
    ~HwModule();

    void push_message(std::string module_name, std::shared_ptr<void> payload);
    
    bool get_message_ack(std::string module_name);
    
    virtual std::string module_name() = 0;
    virtual void execute(uint32_t block_idx) {};
    
protected:
    HwMessage get_message(std::string module_name);

private:
    std::map<std::string, HwMessage> messages;
    std::shared_mutex mtx_messages;
    
    std::vector<std::thread> execute_thread_list;
};

#endif

