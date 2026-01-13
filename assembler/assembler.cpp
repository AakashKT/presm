#include "assembler.h"

void asm_runtime_error(std::string err)
{
    throw std::runtime_error("Assembler: " + err);
}

void dasm_runtime_error(std::string err)
{
    throw std::runtime_error("Disassembler: " + err);
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        asm_runtime_error("Usage is ./asm [source_file]");
    }

    Assembler* a = get_assembler();
    a->set_source_file(std::string(argv[1]));
    a->assemble();
    a->disassembly_check();

    a->debug_print();

    return 0;
}