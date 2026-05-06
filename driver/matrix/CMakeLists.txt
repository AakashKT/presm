cmake_minimum_required(VERSION 3.15...4.0)

include_directories(include/)

SET(DRIVER_SOURCE
    matrix.cpp
    $<TARGET_OBJECTS:device>
)

if(PRESM_SHARED_LIB)
    add_library(${DRIVER_LIB} SHARED ${DRIVER_SOURCE})
else()
    add_library(${DRIVER_LIB} STATIC ${DRIVER_SOURCE})
endif()

if(${DEVICE} STREQUAL "preinception")    
    target_include_directories(${DRIVER_LIB} PUBLIC ${PROJECT_SOURCE_DIR}/device/preinception/include/)
else()
    message(FATAL_ERROR "PRESM: Cannot build driver '${DRIVER}' for device '${DEVICE}'!")
    return()
endif()