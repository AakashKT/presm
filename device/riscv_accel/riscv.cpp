#include "riscv.h"

Device* get_device()
{
    return new RISCVAccel();
}

RISCVAccel::RISCVAccel()
{
    this->name = "RISC-V Accelerator";

    this->device_memory = new DeviceMemory(12e9);
}