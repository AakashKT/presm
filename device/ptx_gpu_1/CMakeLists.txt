cmake_minimum_required(VERSION 3.15...4.0)

SET(DEVICE_CODE
    ptx_gpu.cpp
    ptx_gpu_asm.cpp
)

target_sources(device PUBLIC
    ${DEVICE_CODE}
)
target_include_directories(device PUBLIC include/)