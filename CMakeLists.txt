cmake_minimum_required(VERSION 3.15...4.0)

project(presm VERSION 0.1
        DESCRIPTION "PRE-Silicon Modelling"
        LANGUAGES C CXX CUDA)

# Configuration variables
set(PRESM_SHARED_LIB TRUE CACHE STRING "PRESM as a shared or a static library?")

set(DEVICE "ptx_gpu_1" CACHE STRING "Which device to use? (default: ptx_gpu_1, see device/ folder)")
set(DRIVER "cuda" CACHE STRING "Which driver to use? (default: cuda, see driver/ folder)")

# General includes
include_directories(include)
include_directories(device/include)

# Add device directory
add_subdirectory(device/)

if(PRESM_SHARED_LIB)
    message(STATUS "PRESM: Building as shared library")
else()
    message(STATUS "PRESM: Building as static library")
endif()

# Add driver directory
if(${DRIVER} MATCHES "cuda")
    message(STATUS "PRESM: Compiling with CUDA driver")

    # The actual driver
    add_subdirectory(driver/cuda)

    # Verification tests
    add_subdirectory(verification/cuda)
elseif(${DRIVER} MATCHES "onnx")
    message(STATUS "PRESM: Compiling with ONNX driver")

    # The actual driver
    add_subdirectory(driver/onnx)

    # Verification tests
    add_subdirectory(verification/onnx)
else()
    message(FATAL_ERROR "PRESM: Driver named ${DRIVER} not found!")
    return()
endif()