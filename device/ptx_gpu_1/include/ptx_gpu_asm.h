#ifndef PTX_GPU_1_ASM_H
#define PTX_GPU_1_ASM_H

#include "assembler.h"

class PTXGPUAssembler : public Assembler {
public:
    PTXGPUAssembler(std::string source_isa_file);

    void preprocess();

private:
    std::vector<string> source;
    
    std::vector<string> disassembly;
    std::vector<uint32_t> bin;
};

#endif