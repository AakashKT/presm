cmake_minimum_required(VERSION 3.15...4.0)

find_package(CUDAToolkit REQUIRED)

include_directories(include/)

add_executable(verification
            app.cpp
            sanity/sanity.cpp
        )

target_link_libraries(verification PRIVATE 
            CUDA::cuda_driver
        )