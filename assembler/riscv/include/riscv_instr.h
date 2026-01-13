#ifndef RISCV_INSTR_H
#define RISCV_INSTR_H

#include "assembler.h"
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

#endif