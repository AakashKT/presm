#ifndef HW_CP_H
#define HW_CP_H

#include "hw_module.h"
#include "defs.h"

class HwCp : public HwModule {
public:
    HwCp(HwModule* hw_interface_module);
    
    std::string module_name() override;
    void execute(uint32_t block_idx) override;

private:
    Logger* log;
    HwModule* hw_interface_module;

    uint8_t tx_id = 0;
};

#endif