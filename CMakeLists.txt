cmake_minimum_required(VERSION 3.15...4.0)

project(presm VERSION 0.1
        DESCRIPTION "PRE-Silicon Modelling"
        LANGUAGES C CXX CUDA)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 20)

# Configuration variables
set(PRESM_SHARED_LIB TRUE CACHE STRING "PRESM as a shared or a static library?")

set(DEVICE "ptx_gpu_1" CACHE STRING "Which device to use? (default: ptx_gpu_1, see device/ folder)")
set(DRIVER "cuda" CACHE STRING "Which driver to use? (default: cuda, see driver/ folder)")

# General includes
include_directories(include)
include_directories(device/include)
include_directories(assembler/include)

# Add device directory
add_subdirectory(device/)

# Add assembler directory
add_subdirectory(assembler/)

# Verification tests
add_subdirectory(verification/)

if(PRESM_SHARED_LIB)
    message(STATUS "PRESM: Building as shared library")
else()
    message(STATUS "PRESM: Building as static library")
endif()

# Add driver directory
if(${DRIVER} STREQUAL "c_accel")
    message(STATUS "PRESM: Compiling with COMPUTE ACCELERATOR driver")

    # The actual driver
    add_subdirectory(driver/c_accel)
else()
    message(FATAL_ERROR "PRESM: Driver named ${DRIVER} not found!")
    return()
endif()

# Make packages
set_target_properties(driver PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/driver/${DRIVER}/include/${DRIVER}.h)
install(TARGETS driver 
        LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/packages/${DRIVER}/lib/
        PUBLIC_HEADER DESTINATION ${PROJECT_SOURCE_DIR}/packages/${DRIVER}/include/
)