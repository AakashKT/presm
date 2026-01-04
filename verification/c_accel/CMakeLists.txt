cmake_minimum_required(VERSION 3.15...4.0)

SET(VERIFICATION_SOURCES
    c_accel.cpp
    sanity/sanity.cpp
    sanity/sanity_kernel.cpp
)

target_sources(verification PUBLIC
    ${VERIFICATION_SOURCES}
)
target_include_directories(verification PUBLIC include/)