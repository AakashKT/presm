cmake_minimum_required(VERSION 3.15...4.0)

include_directories(include/)

add_library(driver SHARED
            cuda_api.cpp
            driver_entrypoint.cpp
        )
