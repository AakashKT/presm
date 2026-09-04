cmake_minimum_required(VERSION 3.15...4.0)

add_library(device OBJECT
            device.cpp
            serial_device.cpp
            device_memory.cpp
            host_resident_device_memory.cpp
            hw_module.cpp
        )
target_include_directories(device PUBLIC include/)
target_link_libraries(device
	PRIVATE nlohmann_json::nlohmann_json
)

if(PRESM_SHARED_LIB)
    set_property(TARGET device PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

set(DEVICE_NAME ${DEVICE}) 
configure_file(
    "${PROJECT_SOURCE_DIR}/device/include/config_device.h.in" 
    "${PROJECT_SOURCE_DIR}/device/include/config_device.h"
)

if(${DEVICE} STREQUAL "preinception")
    message(STATUS "PRESM: Building device 'preinception'")
    add_subdirectory(preinception/)
else()
    message(FATAL_ERROR "PRESM: Device '${DEVICE}' not found!")
    return()
endif()