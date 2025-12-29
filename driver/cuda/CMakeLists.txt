cmake_minimum_required(VERSION 3.15...4.0)

include_directories(include/)

SET(DRIVER_SOURCE
    cuda_api.cpp
    driver_entrypoint.cpp
    $<TARGET_OBJECTS:device>
)

if(PRESM_SHARED_LIB)
    add_library(driver SHARED ${DRIVER_SOURCE})
else()
    add_library(driver STATIC ${DRIVER_SOURCE})
endif()
