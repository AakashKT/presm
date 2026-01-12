#include "c_accel.h"
#include "driver_entrypoint.h"
#include "assembler.h"

Device *presm_device = nullptr;
Assembler *caAsm = nullptr;

void caDeviceInit()
{
    presm_device = get_device();
    caAsm = get_assembler();
}

void caSetKernel(std::string kernel_file)
{
    caAsm->set_source_file(kernel_file);

    caAsm->assemble();
    // caAsm->debug_print();
}

void caCopyHtoD(void* src, caDeviceBuffer& dest, size_t size_in_bytes)
{

}

void caCopyDtoH(caDeviceBuffer& src, void* dest, size_t size_in_bytes)
{

}

void caSetKernelParams(uint32_t param_loc, caDeviceBuffer& buffer)
{

}

void caKernelRun()
{

}

void caSync()
{

}

void* caGetKernelOutput()
{
    return nullptr;
}

void caDebugCheckDisassembly()
{
    caAsm->disassemble_check();
}

caDeviceBuffer::caDeviceBuffer(size_t size_in_bytes)
    : size_in_bytes(size_in_bytes)
{

}