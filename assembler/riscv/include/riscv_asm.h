#ifndef RISCV_ASM_H
#define RISCV_ASM_H

#include "assembler.h"
#include "riscv_instr.h"
#include "isa_defs/riscv_defs.h"

class RiscvAssembler : public Assembler {
public:
    RiscvAssembler() {};

    void set_source_file(std::string file_name) override;
    uint32_t estimated_binary_size_in_bytes() override;

    void assemble() override;
    std::vector<uint32_t> get_binary() override;

    void debug_print() override;
    void disassembly_check() override;

private:
    bool is_label(std::string line);
    bool is_instruction(std::string line);
    void preprocess();

    void check_instr_eq_either(RiscvInstr instr, std::string s1, std::string s2, std::string s3);
    void check_instr_eq_either(RiscvInstr instr, std::string s1, std::string s2);
    bool check_instr_eq(RiscvInstr instr, std::string s1, bool exit = true);
    void instr_check(RiscvInstr& instr);
    void operands_check(RiscvInstr& instr);

    std::string source_file = "";
    std::ifstream source_file_stream;

    std::vector<std::pair<std::string, uint32_t>> asm_clean;
    std::map<std::string, std::vector<RiscvInstr>> asm_instr;
    std::map<std::string, uint32_t> label_addresses;
};

#endif