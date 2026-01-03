cmake_minimum_required(VERSION 3.15...4.0)

find_package(CUDAToolkit REQUIRED)

set(HOST_CODE
    cuda_verification.cpp
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
set_property(TARGET ptx PROPERTY CMAKE_CUDA_FLAGS O0)
install(TARGETS ptx
    OBJECTS DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/
)

target_sources(verification PUBLIC
    ${HOST_CODE}
)
target_include_directories(verification PUBLIC include/)
target_link_libraries(verification PRIVATE 
    CUDA::cuda_driver
)