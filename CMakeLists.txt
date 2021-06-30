cmake_minimum_required(VERSION 3.13)

PROJECT(hgardenpi VERSION "0.10.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(RASPBIAN_ROOTFS $ENV{RASPBIAN_ROOTFS})

configure_file(config.h.in ../src/config.h)
include_directories("${RASPBIAN_ROOTFS}/usr/local/include")
include_directories("${RASPBIAN_ROOTFS}/usr/include")
include_directories("3thparty/include/date")
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

target_link_libraries(hgardenpi
                        wiringPi
                        wiringPiDev
                        pthread
                        mosquittopp
                        )