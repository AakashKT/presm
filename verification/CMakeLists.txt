cmake_minimum_required(VERSION 3.15...4.0)

add_executable(verification
    app.cpp
)

target_include_directories(verification PUBLIC include/)

if(${DRIVER} MATCHES "matrix")
    message(STATUS "PRESM: Using MATRIX verification tests")
    add_subdirectory(matrix/)

elseif(${DRIVER} MATCHES "mlpin")
    message(STATUS "PRESM: Using MLPIN verification tests")
    add_subdirectory(mlpin/)

else()
    message(FATAL_ERROR "PRESM: Verification test for driver ${DRIVER} not found!")
    return()
endif()