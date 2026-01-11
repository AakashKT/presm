#ifndef RISCV_DEFS_H
#define RISCV_DEFS_H

enum INSTR_TYPE {
    NONE_TYPE=0, R_TYPE, I_TYPE, B_TYPE, S_TYPE, U_TYPE, J_TYPE
};

enum OPCODE {
    R_OPCODE = 0x0,
    I_OPCODE = 0x1,
    B_OPCODE = 0x2,
    S_OPCODE = 0x3,
    LUI = 0x4,
    AUIPC = 0x5,
    JAL = 0x6,
    JALR = 0x7,
    BRANCH = 0x8,
    LOAD = 0x9,
    STORE = 0xA,
};

enum FUNCT3 {
    // Integer register-immediate
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#_integer_register_immediate_instructions
    ADDI = 0x0,
    SLTI = 0x1,
    SLTIU = 0x2,
    ANDI = 0x3,
    ORI = 0x4,
    XORI = 0x5,
    SLLI = 0x6,
    SRLI = 0x7,
    SRAI = 0x7,
    // Integer register-register
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#_integer_register_register_instructions
    ADD = 0x0,
    SLT = 0x1,
    SLTU = 0x2,
    AND = 0x3,
    OR = 0x4,
    XOR = 0x5,
    SUB = 0x6,
    SLL = 0x0,
    SRL = 0x1,
    SRA = 0x2,
    // Control transfer
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#_control_transfer_instructions
    BEQ = 0x0,
    BNE = 0x1,
    BLT = 0x2,
    BLTU = 0x3,
    BGE = 0x4,
    BGEU = 0x5,
    // Load
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#ldst
    LW = 0x0,
    LH = 0x1,
    LHU = 0x2,
    LB = 0x3,
    LBU = 0x4,
    // Store
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#ldst
    SW = 0x0,
    SH = 0x1,
    SHU = 0x2,
    SB = 0x3,
    SBU = 0x4,
};

enum REG_MAP {
    // https://shakti.org.in/docs/risc-v-asm-manual.pdf
    X0 = 0x0,
    RA = 0x1,
    SP = 0x2, 
    GP = 0x3,
    TP = 0x4,
    T0 = 0x5,
    T1 = 0x6,
    T2 = 0x7,
    S0 = 0x8,
    S1 = 0x9,
    A0 = 0xA,
    S2 = 0x12,
    T3 = 0x1C,
};

union RISCV_INSTR_BIN {

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
        uint32_t imm_4_0 : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t imm_11_5 : 7;
    } s_type;

    struct {
        uint32_t const_bits : 2;
        uint32_t opcode : 5;
        uint32_t imm_11 : 1;
        uint32_t imm_4_1 : 4;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t imm_10_5 : 6;
        uint32_t imm_12 : 1;
    } b_type;

    struct {
        uint32_t const_bits : 2;
        uint32_t opcode : 5;
        uint32_t rd : 5;
        uint32_t imm : 20;
    } u_type;

    struct {
        uint32_t const_bits : 2;
        uint32_t opcode : 5;
        uint32_t rd : 5;
        uint32_t imm_19_12 : 8;
        uint32_t imm_11 : 1;
        uint32_t imm_10_1 : 10;
        uint32_t imm_20 : 1;
    } j_type;

    uint32_t bits = 0;

    void init_opcode(uint32_t opcode) { r_type.const_bits = 0x3; r_type.opcode = opcode; };
    void init_funct3(uint32_t funct3) { r_type.funct3 = funct3; };
    void init_funct7(uint32_t funct7) { r_type.funct7 = funct7; };
};

#endif