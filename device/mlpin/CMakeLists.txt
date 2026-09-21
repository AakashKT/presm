cmake_minimum_required(VERSION 3.15...4.0)

if(${DEVICE_TYPE} STREQUAL "functional")

    SET(DEVICE_CODE
        functional_impl.cpp
    )

elseif(${DEVICE_TYPE} STREQUAL "serial")

    SET(DEVICE_CODE

    )

    
else()
    message(FATAL_ERROR "PRESM: Device '${DEVICE}' does not define a '${DEVICE_TYPE}' variant!")
    return()
endif()

set(HOST_RESIDENT_MEM_SIZE ${DEVICE_MEM_SIZE_IN_BYTES})
configure_file(
    "${PROJECT_SOURCE_DIR}/device/mlpin/include/config_mlpin.h.in" 
    "${PROJECT_SOURCE_DIR}/device/mlpin/include/config_mlpin.h"
)

target_sources(device PUBLIC
    ${DEVICE_CODE}
)
target_include_directories(device PUBLIC include/)