cmake_minimum_required(VERSION 3.15...4.0)

include_directories(include/)

SET(DEVICE_SOURCE_FILES
    device.cpp
    memory.cpp
    assembler.cpp
)

if(${DEVICE} MATCHES "ptx_gpu_1")
    message(STATUS "PRESM: Using device ptx_gpu_1")
    SET(DEVICE_ID 0)
        
    include_directories(ptx_gpu_1/include)

    list(APPEND DEVICE_SOURCE_FILES
        ptx_gpu_1/ptx_gpu.cpp
    )
else()
    message(FATAL_ERROR "PRESM: Device ${DEVICE} not found!")
    return()
endif()

add_library(device OBJECT
            ${DEVICE_SOURCE_FILES}
        )
target_compile_definitions(device PRIVATE DEVICE_ID=DEVICE_ID)

if(PRESM_SHARED_LIB)
    set_property(TARGET device PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()