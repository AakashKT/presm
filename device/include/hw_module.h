#ifndef HW_MODULE_H
#define HW_MODULE_H

#include "common.h"
#include "logging.h"

class HwModule {
public:
    HwModule(uint32_t num_parallel_blocks);
    ~HwModule();

    void push_message(std::string module_name, void* payload);
    
    virtual std::string module_name() = 0;
    virtual void execute(uint32_t block_idx) = 0;
    
protected:
    void* get_message(std::string module_name);

private:
    std::map<std::string, void*> messges;
    std::shared_mutex mtx_messages;
    
    std::vector<std::thread> execute_thread_list;
};

#endif

