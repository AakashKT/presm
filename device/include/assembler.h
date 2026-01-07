#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "common.h"

class Assembler {
public:    
    virtual void assemble() = 0;
    virtual uint32_t estimated_binary_size_in_bytes() = 0;
    virtual void set_source_file(std::string file_name) = 0;
    // virtual void* get_assembled() = 0;

    // virtual void disassemble() = 0;
    // virtual void get_disassembled() = 0;
};

Assembler* get_assembler();

#endif