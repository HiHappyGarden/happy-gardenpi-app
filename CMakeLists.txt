cmake_minimum_required(VERSION 3.13)

PROJECT(hgardenpi VERSION "0.10.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(RASPBIAN_ROOTFS $ENV{RASPBIAN_ROOTFS})

#include date of HowardHinnant
include(ExternalProject)
ExternalProject_Add(
  date
  URL https://github.com/HowardHinnant/date/archive/refs/tags/v3.0.1.tar.gz
  SOURCE_DIR      "${CMAKE_BINARY_DIR}/date"
  BINARY_DIR      "${CMAKE_BINARY_DIR}/date"
  CMAKE_ARGS      "-DBUILD_TZ_LIB=ON"
  INSTALL_COMMAND ""
)

configure_file(config.h.in ../src/config.h)
include_directories("${RASPBIAN_ROOTFS}/usr/local/include")
include_directories("${RASPBIAN_ROOTFS}/usr/include")
link_directories("${RASPBIAN_ROOTFS}/usr/local/lib")
link_directories("${RASPBIAN_ROOTFS}/usr/lib")


add_executable(hgardenpi
    src/controllers/mqttclient.cpp
    src/controllers/mqttclient.hpp
    src/controllers/scheduler.cpp
    src/controllers/scheduler.hpp
    src/utilities/singleton.hpp
    src/main.cpp
    src/globals.cpp
    src/globals.hpp
)

#aggiungo le dipendenze
add_dependencies(hgardenpi date)

target_link_libraries(hgardenpi
                        wiringPi
                        wiringPiDev
                        pthread
                        mosquittopp
                        )