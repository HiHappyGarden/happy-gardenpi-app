cmake_minimum_required(VERSION 3.16)

PROJECT(hgardenpi VERSION "0.10.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(RASPBIAN_ROOTFS "$ENV{HOME}/raspberrypi/rootfs")
set(FILE_LOCK_PATH "/tmp/hgardenpi.pid")
set(FILE_CONFIG "./config.json")
set(BROKER_HOST "passy-raspberrypi")
set(BROKER_PORT 1883)
set(BROKER_USER "hgardepi")
set(BROKER_PASSWD "hgardepi")



include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


configure_file(config.h.in ../src/config.h)
include_directories("${RASPBIAN_ROOTFS}/usr/local/include")
include_directories("${RASPBIAN_ROOTFS}/usr/include")
include_directories("3thparty/include/date")
link_directories("${RASPBIAN_ROOTFS}/usr/local/lib")
link_directories("${RASPBIAN_ROOTFS}/usr/lib")


add_executable(hgardenpi
    src/clients/mqttclientlocalsub.cpp
    src/clients/mqttclientlocalsub.hpp
    src/interfaces/mqttclient.hpp
        src/pods/configinfo.cpp
    src/pods/configinfo.hpp
    src/pods/deviceinfo.hpp
    src/services/configservice.cpp
    src/services/configservice.hpp
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
    src/pods/deviceinfo.hpp
        )

target_link_libraries(hgardenpi
                        wiringPi
                        wiringPiDev
                        pthread
                        mosquitto
                        nlohmann_json::nlohmann_json
                        )