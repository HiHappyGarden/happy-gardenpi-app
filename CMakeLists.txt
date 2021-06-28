cmake_minimum_required(VERSION 3.13)

PROJECT(hgardenpi VERSION "0.10.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(RASPBIAN_ROOTFS $ENV{RASPBIAN_ROOTFS})

configure_file(config.h.in ../src/config.h)
include_directories("${RASPBIAN_ROOTFS}/usr/local/include")
include_directories("${RASPBIAN_ROOTFS}/usr/include")
link_directories("${RASPBIAN_ROOTFS}/usr/local/lib")
link_directories("${RASPBIAN_ROOTFS}/usr/lib")

add_executable(hgardenpi
    src/commons.cpp
    src/commons.hpp
    src/main.cpp
)


target_link_libraries(hgardenpi
                        wiringPi
                        wiringPiDev
                        pthread
                        mosquittopp
                        )