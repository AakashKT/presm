cmake_minimum_required(VERSION 3.15...4.0)

include_directories(include/)

set(HOST_CODE
    app.cpp
    sanity/sanity.cpp
)

add_executable(verification
    ${HOST_CODE}
)

target_include_directories(verification PRIVATE
    "${CMAKE_SOURCE_DIR}/extern/onnxruntime/onnxruntime-linux-x64-1.23.2/include"
)

target_link_libraries(verification PRIVATE 
    ${CMAKE_SOURCE_DIR}/extern/onnxruntime/onnxruntime-linux-x64-1.23.2/lib/libonnxruntime.so
)