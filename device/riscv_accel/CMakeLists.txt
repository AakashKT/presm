cmake_minimum_required(VERSION 3.15...4.0)

SET(DEVICE_CODE
    riscv.cpp
)

target_sources(device PUBLIC
    ${DEVICE_CODE}
)
target_include_directories(device PUBLIC include/)