cmake_minimum_required(VERSION 3.15...4.0)

target_sources(assembler PUBLIC
    riscv_asm.cpp
    riscv_instr.cpp
)
target_include_directories(assembler PUBLIC include/)
target_include_directories(device PUBLIC include/isa_defs/)