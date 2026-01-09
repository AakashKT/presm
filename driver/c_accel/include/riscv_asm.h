#ifndef RISCV_ASM_H
#define RISCV_ASM_H

#include "common.h"
#include "isa_defs/riscv_defs.h"

enum INSTR_TYPE {
    NONE_TYPE=0, R_TYPE, I_TYPE, S_TYPE, U_TYPE
};

union RISCV_INSTR {

    struct {
        uint32_t const_bits : 2;
        uint32_t opcode : 5;
        uint32_t rd : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t funct7 : 7;
    } r_type;

    struct {
        uint32_t const_bits : 2;
        uint32_t opcode : 5;
        uint32_t rd : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t imm : 12;
    } i_type;

    struct {
        uint32_t const_bits : 2;
        uint32_t opcode : 5;
        uint32_t imm_low : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t imm_hi : 7;
    } s_type;

    struct {
        uint32_t const_bits : 2;
        uint32_t opcode : 5;
        uint32_t rd : 5;
        uint32_t imm : 20;
    } u_type;

    uint32_t bits = 0;

    void init_r_type() { r_type.const_bits = 0x3; r_type.opcode = R_OPCODE; };
    void init_i_type() { i_type.const_bits = 0x3; i_type.opcode = I_OPCODE; };
    void init_s_type() { s_type.const_bits = 0x3; s_type.opcode = S_OPCODE; };
    void init_u_type() { u_type.const_bits = 0x3; };
};

class RiscvInstr {
public:
    RiscvInstr(std::string instr);

    void process_op(std::string opcode);
    void process_operand(uint32_t op_idx, std::string operand);

    std::string get_instr_string() { return this->instr; };
    uint32_t get_instr_binary() { return this->instr_bin.bits; };
    INSTR_TYPE get_instr_type() { return this->type; };

private:
    std::string instr;

    INSTR_TYPE type = NONE_TYPE;
    RISCV_INSTR instr_bin;
};

class CaAssembler {
public:
    CaAssembler() {};

    void set_source_file(std::string file_name);
    void assemble();
    uint32_t estimated_binary_size_in_bytes();

    void print_instructions();

private:
    std::string source_file = "";
    std::ifstream source_file_stream;

    std::map<std::string, std::vector<RiscvInstr>> asm_instr;
};

#endif