cmake_minimum_required(VERSION 3.15...4.0)

add_library(device OBJECT
            memory.cpp
        )
target_include_directories(device PUBLIC include/)

if(PRESM_SHARED_LIB)
    set_property(TARGET device PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

if(${DEVICE} MATCHES "ptx_gpu_1")
    message(STATUS "PRESM: Using device ptx_gpu_1")
        
    add_subdirectory(ptx_gpu_1/)
elseif(${DEVICE} MATCHES "riscv_accel")
    message(STATUS "PRESM: Using device RISC-V accelerator")
        
    add_subdirectory(riscv_accel/)
else()
    message(FATAL_ERROR "PRESM: Device ${DEVICE} not found!")
    return()
endif()