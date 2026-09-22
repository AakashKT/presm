cmake_minimum_required(VERSION 3.15...4.0)

if(${DEVICE_TYPE} STREQUAL "functional")

    SET(DEVICE_CODE
        functional_impl.cpp
        hw_interface.cpp
        hw_cp.cpp
    )

elseif(${DEVICE_TYPE} STREQUAL "serial")

    SET(DEVICE_CODE
        serial_impl.cpp
    )

    
else()
    message(FATAL_ERROR "PRESM: Device '${DEVICE}' does not define a '${DEVICE_TYPE}' variant!")
    return()
endif()

set(HOST_RESIDENT_MEM_SIZE ${DEVICE_MEM_SIZE_IN_BYTES})
configure_file(
    "${PROJECT_SOURCE_DIR}/device/preinception/include/config_preinception.h.in" 
    "${PROJECT_SOURCE_DIR}/device/preinception/include/g_config_preinception.h"
)

target_sources(device PUBLIC
    ${DEVICE_CODE}
)
target_include_directories(device PUBLIC include/)