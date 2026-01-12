cmake_minimum_required(VERSION 3.15...4.0)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(RISC_V_COMPILER_BIN "${PROJECT_SOURCE_DIR}/build_riscv_compiler/windows/xpack/bin/riscv-none-elf-g++.exe")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(RISC_V_COMPILER_BIN "${PROJECT_SOURCE_DIR}/build_riscv_compiler/linux/bin/riscv32-unknown-elf-g++")
endif()

SET(VERIFICATION_KERNELS
    sanity/sanity_kernel.cpp
    conditionals/cond_kernel.cpp
    function_call/fc_kernel.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(VERIFICATION_KERNELS_COMPILED_OUTPUTS
        ${PROJECT_SOURCE_DIR}/build/verification/Release/sanity_kernel.s
        ${PROJECT_SOURCE_DIR}/build/verification/Release/cond_kernel.s
        ${PROJECT_SOURCE_DIR}/build/verification/Release/fc_kernel.s
    )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(VERIFICATION_KERNELS_COMPILED_OUTPUTS
        ${PROJECT_SOURCE_DIR}/build/verification/sanity_kernel.s
        ${PROJECT_SOURCE_DIR}/build/verification/cond_kernel.s
        ${PROJECT_SOURCE_DIR}/build/verification/fc_kernel.s
    )
endif()

SET(VERIFICATION_SOURCES
    c_accel_app.cpp
    sanity/sanity.cpp
    conditionals/cond.cpp
    function_call/fc.cpp
    
    ${VERIFICATION_KERNELS}
)

target_sources(verification PUBLIC
    ${VERIFICATION_SOURCES}
)
target_include_directories(verification PUBLIC include/)
target_include_directories(verification PUBLIC ${PROJECT_SOURCE_DIR}/driver/c_accel/include/)

target_link_libraries(verification PUBLIC driver)

# Compile kernels with the RISC-V compiler
foreach(item IN ZIP_LISTS VERIFICATION_KERNELS VERIFICATION_KERNELS_COMPILED_OUTPUTS)
    add_custom_command(
        OUTPUT ${item_1}
        COMMAND ${RISC_V_COMPILER_BIN} 
                -S ${PROJECT_SOURCE_DIR}/verification/c_accel/${item_0} 
                -o ${item_1}
    )
endforeach()

add_custom_target(compiled_kernels ALL DEPENDS ${VERIFICATION_KERNELS_COMPILED_OUTPUTS})
add_dependencies(verification compiled_kernels)

