cmake_minimum_required(VERSION 3.15...4.0)

SET(VERIFICATION_SOURCES
    matrix_app.cpp
    sanity/sanity.cpp
    addition/addition.cpp
    multiplication/multiplication.cpp
    divide_power_2/divide_power_2.cpp
    multiple_commands/multiple_commands.cpp
)

target_sources(verification PUBLIC
    ${VERIFICATION_SOURCES}
)
target_include_directories(verification PUBLIC include/)
target_include_directories(verification PUBLIC ${PROJECT_SOURCE_DIR}/driver/matrix/include/)

target_link_libraries(verification PUBLIC ${DRIVER_LIB})