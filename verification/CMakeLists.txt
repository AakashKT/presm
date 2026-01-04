cmake_minimum_required(VERSION 3.15...4.0)

add_executable(verification
    app.cpp
)

target_include_directories(verification PUBLIC include/)

if(${DRIVER} MATCHES "cuda")
    message(STATUS "PRESM: Using CUDA verification tests")

    add_subdirectory(cuda/)
elseif(${DRIVER} MATCHES "c_accel")
    message(STATUS "PRESM: Using COMPUTE ACCELERATOR verification tests")

    add_subdirectory(c_accel/)
else()
    message(FATAL_ERROR "PRESM: Verification test for driver ${DRIVER} not found!")
    return()
endif()