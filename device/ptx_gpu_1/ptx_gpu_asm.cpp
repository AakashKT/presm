#include "ptx_gpu_asm.h"

Assembler* get_assembler()
{
    return new PTXGPUAssembler("NULL");
}

PTXGPUAssembler::PTXGPUAssembler(std::string source_isa_file)
{
    std::ifstream file(source_isa_file);
    std::string str, substr; 
    while (std::getline(file, str))
    {
        // Ignore single-line comments
        substr = str.substr(0, 2);
        bool is_comment = substr == "//";
        if(!is_comment)
            this->source.push_back(str);
    }
}

void PTXGPUAssembler::preprocess()
{
    for(auto& line : this->source) {
        // Kernel entrypoints and their metadata
        
    }
}

void PTXGPUAssembler::assemble()
{
    this->preprocess();
}