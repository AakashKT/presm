#ifndef RISCV_ASM_H
#define RISCV_ASM_H

#include "common.h"
#include "isa_defs/riscv_defs.h"

class RiscvInstr {
public:
    RiscvInstr(std::string instr, uint32_t instr_address, std::map<std::string, uint32_t>* label_addresses_ptr);

    void assemble_binary();

    void assemble_op();
    void assemble_operand(uint32_t op_idx, std::string operand);

    std::string& get_instr_string() { return this->instr; };
    std::vector<std::string>& get_operand_strings() { return this->operands; };
    std::vector<std::string>& get_processed_operand_strings() { return this->operands_; };
    uint32_t get_instr_binary() { return this->instr_bin.bits; };
    INSTR_TYPE get_instr_type() { return this->type; };

private:
    std::string instr;
    std::vector<std::string> operands, operands_;

    uint32_t instr_address;
    INSTR_TYPE type = NONE_TYPE;
    RISCV_INSTR_BIN instr_bin;
    
    std::map<std::string, uint32_t>* label_addresses_ptr;

    std::regex operands_reg_offset_regex = std::regex(R"~((.*)\((.*)\))~");
};

class RiscvAssembler {
public:
    RiscvAssembler() {};

    void set_source_file(std::string file_name);
    bool is_label(std::string line);
    bool is_instruction(std::string line);
    void preprocess();
    void assemble();
    uint32_t estimated_binary_size_in_bytes();

    void print_instructions();

    void check_instr_eq_either(RiscvInstr instr, std::string s1, std::string s2, std::string s3);
    void check_instr_eq_either(RiscvInstr instr, std::string s1, std::string s2);
    bool check_instr_eq(RiscvInstr instr, std::string s1, bool exit = true);
    void instr_check(RiscvInstr& instr);
    void disassemble_check();

private:
    std::string source_file = "";
    std::ifstream source_file_stream;

    std::vector<std::pair<std::string, uint32_t>> asm_clean;
    std::map<std::string, std::vector<RiscvInstr>> asm_instr;
    std::map<std::string, uint32_t> label_addresses;
};

#endif