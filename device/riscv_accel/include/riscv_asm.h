#ifndef RISCV_ASM_H
#define RISCV_ASM_H

#include "assembler.h"

class RiscvInstrs {
public:
    RiscvInstrs() {};

    void add_instruction(std::string instr);
    void instruction_operand(uint32_t op_num, std::string op);
    void clear();


    std::vector<std::string> instruction;
    std::vector<uint32_t> num_ops;
    std::vector<std::string> op1, op2, op3, op4, op5;

    int current_idx = -1;
};

class CaAsm : public Assembler {
public:
    CaAsm() {};

    void set_source_file(std::string file_name) override;
    void assemble() override;
    uint32_t estimated_binary_size_in_bytes() override;

private:
    void preprocess();
    void print_instructions();

    std::string source_file = "";
    std::ifstream source_file_stream;

    std::map<std::string, RiscvInstrs> asm_instr;
};

#endif