cmake_minimum_required(VERSION 3.15...4.0)

SET(VERIFICATION_KERNELS
    sanity/sanity_kernel.cpp
    vector_add/vector_add_kernel.cpp
)

SET(VERIFICATION_KERNELS_COMPILED_OUTPUTS
    sanity_kernel.s
    vector_add_kernel.s
)

SET(VERIFICATION_SOURCES
    c_accel_app.cpp
    sanity/sanity.cpp
    
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
        COMMAND ${PROJECT_SOURCE_DIR}/build_riscv_compiler/bin/riscv32-unknown-elf-g++ 
                -S ${PROJECT_SOURCE_DIR}/verification/c_accel/${item_0} 
                -o ${PROJECT_SOURCE_DIR}/build/verification/${item_1}
    )
endforeach()

add_custom_target(compiled_kernels ALL DEPENDS ${VERIFICATION_KERNELS_COMPILED_OUTPUTS})
add_dependencies(verification compiled_kernels)

