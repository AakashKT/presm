cmake_minimum_required(VERSION 3.15...4.0)

SET(VERIFICATION_SOURCES
    mlpin_app.cpp
)

target_sources(verification PUBLIC
    ${VERIFICATION_SOURCES}
)
target_include_directories(verification PUBLIC include/)
target_include_directories(verification PUBLIC ${PROJECT_SOURCE_DIR}/driver/mlpin/include/)

target_link_libraries(verification PUBLIC ${DRIVER_LIB})