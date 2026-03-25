cmake_minimum_required(VERSION 3.15...4.0)

add_library(device OBJECT
            device.cpp
            uart_device.cpp
        )
target_include_directories(device PUBLIC include/)
target_link_libraries(device
	PRIVATE nlohmann_json::nlohmann_json
)

if(PRESM_SHARED_LIB)
    set_property(TARGET device PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

if(${DEVICE} STREQUAL "inception")
    message(STATUS "PRESM: Building device 'inception' - A RISC-V compute accelerator")
    
    add_subdirectory(inception/)
elseif(${DEVICE} STREQUAL "preinception")
    message(STATUS "PRESM: Building device 'preinception' - A matrix operations accelerator")
    
    add_subdirectory(preinception/)
else()
    message(FATAL_ERROR "PRESM: Device '${DEVICE}' not found!")
    return()
endif()