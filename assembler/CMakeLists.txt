cmake_minimum_required(VERSION 3.15...4.0)

add_library(assembler OBJECT
    assembler.cpp
)
target_include_directories(assembler PUBLIC include/)

if(PRESM_SHARED_LIB)
    set_property(TARGET assembler PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

if(${DEVICE} STREQUAL "inception")
    message(STATUS "PRESM: Using RISC-V assembler")
        
    add_subdirectory(riscv/)
else()
    message(FATAL_ERROR "PRESM: No assembler found for ${DEVICE}!")
    return()
endif()

add_executable(asm
    $<TARGET_OBJECTS:assembler>
)