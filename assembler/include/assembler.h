#ifndef ASM_H
#define ASM_H

#include "common.h"

class Assembler {
public:
    virtual void set_source_file(std::string file_name) = 0;
    virtual void assemble() = 0;
    virtual uint32_t estimated_binary_size_in_bytes() = 0;

    virtual void debug_print() = 0;
    virtual void disassemble_check() = 0;
};

Assembler* get_assembler();

#endif