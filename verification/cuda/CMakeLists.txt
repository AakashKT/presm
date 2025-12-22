cmake_minimum_required(VERSION 3.15...4.0)

find_package(CUDAToolkit REQUIRED)

include_directories(include/)

set(HOST_CODE
    app.cpp
    sanity/sanity.cpp
    vector_add/vector_add.cpp
)
set(DEVICE_CODE
    sanity/sanity.cu
    vector_add/vector_add.cu
)

add_library(ptx OBJECT
    ${DEVICE_CODE}
)
set_property(TARGET ptx PROPERTY CUDA_PTX_COMPILATION ON)
install(TARGETS ptx
    OBJECTS DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/
)

add_executable(verification
    ${HOST_CODE}
)
target_link_libraries(verification PRIVATE 
    CUDA::cuda_driver
)