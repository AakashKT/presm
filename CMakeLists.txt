cmake_minimum_required(VERSION 3.15...4.0)

project(presm VERSION 0.1
        DESCRIPTION "GPU PRE-Silicon Modelling"
        LANGUAGES C CXX CUDA)

# Configuration variables
set(DEVICE "ptx_gpu_1" CACHE STRING "Which device to use? (default: ptx_gpu_1, see device/ folder)")
set(DRIVER "cuda" CACHE STRING "Which driver to use? (default: cuda, see driver/ folder)")

# General includes
include_directories(device/include)

# Add device directory
add_subdirectory(device/)

# Add driver directory
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