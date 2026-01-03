#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "common.h"

class Assembler {
public:
    virtual void assemble() = 0;
    // virtual void* get_assembled() = 0;

    // virtual void disassemble() = 0;
    // virtual void get_disassembled() = 0;
};

Assembler* get_assembler();

#endif