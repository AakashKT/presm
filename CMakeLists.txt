cmake_minimum_required(VERSION 3.15...4.0)

project(presm VERSION 0.1
        DESCRIPTION "GPU PRE-Silicon Modelling"
        LANGUAGES C CXX CUDA)

# Configuration variables
set(DRIVER "cuda" CACHE STRING "Which driver to use? (default: cuda)")

if(${DRIVER} MATCHES "cuda")
    message(STATUS "PRESM: Compiling with CUDA driver")

    # The actual driver
    add_subdirectory(driver/cuda)

    # Verification tests
    add_subdirectory(verification/cuda)
else()
    message(FATAL_ERROR "PRESM: Driver named ${DRIVER} not found!")
    return()
endif()