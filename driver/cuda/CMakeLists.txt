cmake_minimum_required(VERSION 3.15...4.0)

include_directories(include/)

if(PRESM_SHARED_LIB)
    add_library(driver SHARED
                cuda_api.cpp
                driver_entrypoint.cpp
                $<TARGET_OBJECTS:device>
            )
else()
    add_library(driver STATIC
                cuda_api.cpp
                driver_entrypoint.cpp
                $<TARGET_OBJECTS:device>
            )
endif()
