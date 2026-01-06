cmake_minimum_required(VERSION 3.15...4.0)

SET(VERIFICATION_KERNELS
    sanity/sanity_kernel.cpp
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

# RISC-V compiler
# add_custom_command(
#     OUTPUT build/verification/c_accel/sanity_kernel.riscv
#     COMMAND build/riscv-compiler/riscv 
# )