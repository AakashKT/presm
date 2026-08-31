#ifndef HW_MODULE_H
#define HW_MODULE_H

#include "common.h"
#include "logging.h"

class HwModule {
public:
    HwModule();
    ~HwModule();

    void push_message(void* payload);
    void* get_message();
    void connect_to(HwModule& module);

    virtual void execute() = 0;

private:
    std::list<void*> messges;
    std::shared_mutex mtx_messages;
    
    std::list<HwModule*> connections;

    std::thread execute_thread;
};

#endif

