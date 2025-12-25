cmake_minimum_required(VERSION 3.15...4.0)

include_directories(include/)

add_library(device OBJECT
            device.cpp
        )
set_property(TARGET device PROPERTY POSITION_INDEPENDENT_CODE ON)

