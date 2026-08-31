cmake_minimum_required(VERSION 3.15...4.0)

if(${DEVICE_TYPE} STREQUAL "functional")

    SET(DEVICE_CODE
        functional_impl.cpp
        functional_hw_modules.cpp
    )

elseif(${DEVICE_TYPE} STREQUAL "serial")

    SET(DEVICE_CODE
        serial_impl.cpp
    )

    
else()
    message(FATAL_ERROR "PRESM: Device '${DEVICE}' does not define a '${DEVICE_TYPE}' variant!")
    return()
endif()


target_sources(device PUBLIC
    ${DEVICE_CODE}
)
target_include_directories(device PUBLIC include/)