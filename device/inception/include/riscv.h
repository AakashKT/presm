#ifndef RISCV_ACCEL_H
#define RISCV_ACCEL_H

#include "device.h"
#include "host_resident_memory.h"
#include "riscv_defs.h"

class RISCVAccel : public Device {
public:
    RISCVAccel();

    std::string get_name() override { return name; };

private:
    std::string name;
};

#endif