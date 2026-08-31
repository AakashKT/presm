#ifndef FUNCTIONAL_HW_MODULE_H
#define FUNCTIONAL_HW_MODULE_H

#include "hw_module.h"
#include "defs.h"

class HwUartPacket : public HwModule {
public:
    HwUartPacket();

    void execute() override;

private:
    Logger* log;
};

#endif