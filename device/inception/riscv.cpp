#include "riscv.h"

Device* get_device()
{
    return new RISCVAccel();
}

RISCVAccel::RISCVAccel()
{
    this->name = "RISC-V Accelerator";
}