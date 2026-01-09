#ifndef RISCV_DEFS_H
#define RISCV_DEFS_H

enum OPCODE {
    R_OPCODE = 0x0,
    I_OPCODE = 0x1,
    S_OPCODE = 0x2,
    // U_OPCODE
    LUI = 0x3,
    AUIPC = 0x4,
};

enum FUNCT3 {
    // Integer register-immediate
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#_integer_register_immediate_instructions
    ADDI = 0x0,
    SLTI = 0x1,
    ANDI = 0x2,
    ORI = 0x3,
    XORI = 0x4,
    SLLI = 0x5,
    SRLI = 0x6,
    SRAI = 0x7,
    // Integer register-register
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#_integer_register_register_instructions
    ADD = 0x0,
    SLT = 0x1,
    AND = 0x2,
    OR = 0x3,
    XOR = 0x4,
    SLL = 0x5,
    SRL = 0x6,
    SUB = 0x7,
    SRA = 0x7,
};

enum REG_MAP {
    // https://shakti.org.in/docs/risc-v-asm-manual.pdf
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

#endif