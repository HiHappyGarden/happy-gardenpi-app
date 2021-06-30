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
    src/clients/mqttclient.cpp
    src/clients/mqttclient.hpp
    src/services/deviceservice.cpp
    src/services/deviceservice.hpp
    src/services/lockservice.cpp
    src/services/lockservice.hpp
    src/services/logservice.cpp
    src/services/logservice.hpp
    src/services/scheduler.cpp
    src/services/scheduler.hpp
    src/utilities/object.hpp
    src/utilities/singleton.hpp
    src/utilities/stringutils.cpp
    src/utilities/stringutils.hpp
    src/constants.cpp
    src/constants.hpp
    src/globals.cpp
    src/globals.hpp
    src/main.cpp
)

target_link_libraries(hgardenpi
                        wiringPi
                        wiringPiDev
                        pthread
                        mosquittopp
                        )