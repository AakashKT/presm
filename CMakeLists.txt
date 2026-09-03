cmake_minimum_required(VERSION 3.15...4.0)

project(presm VERSION 0.1
        DESCRIPTION "Pre-Silicon Modelling for hardware accelerators"
        LANGUAGES C CXX CUDA)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 20)

# Configuration variables
set(PRESM_SHARED_LIB TRUE CACHE STRING "PRESM as a shared or a static library?")

set(DEVICE "preinception" CACHE STRING "Which device to use? (default: 'preinception', see device/ folder)")
set(DEVICE_TYPE "functional" CACHE STRING "Device type? (default: 'functional')")
set(DRIVER "matrix" CACHE STRING "Which driver to use? (default: 'matrix', see driver/ folder)")

set(DEVICE_MEM_SIZE_IN_BYTES 10000000000 CACHE STRING "Device memory size")
set(BAUD_RATE 9600 CACHE STRING "Host-device communication rate")

set(DRIVER_LIB ${DRIVER}_${DEVICE_TYPE})

# External libraries
add_subdirectory(extern/nlohmann-json/)
include_directories(extern/nlohmann-json/include)

# General includes
include_directories(common/include)
include_directories(device/include)
include_directories(assembler/include)

# Add device directory
add_subdirectory(device/)
target_sources(device PUBLIC common/logging.cpp)

# Verification tests
add_subdirectory(verification/)

if(PRESM_SHARED_LIB)
    message(STATUS "PRESM: Building as shared library")
else()
    message(STATUS "PRESM: Building as static library")
endif()

# Add driver directory
if(${DRIVER} STREQUAL "matrix")
    message(STATUS "PRESM: Compiling with MATRIX driver")
    add_subdirectory(driver/matrix)
else()
    message(FATAL_ERROR "PRESM: Driver named '${DRIVER}' not found!")
    return()
endif()

target_compile_definitions(device PUBLIC LOG_ENABLED=${LOG_ENABLED})
target_compile_definitions(${DRIVER_LIB} PUBLIC LOG_ENABLED=${LOG_ENABLED})

# Make packages
set_target_properties(${DRIVER_LIB} PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/driver/${DRIVER}/include/${DRIVER}.h)
install(TARGETS ${DRIVER_LIB} 
        LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/packages/${DRIVER}/lib/
        PUBLIC_HEADER DESTINATION ${PROJECT_SOURCE_DIR}/packages/${DRIVER}/include/
)