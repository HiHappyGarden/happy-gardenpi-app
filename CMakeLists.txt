cmake_minimum_required(VERSION 3.16)

PROJECT(hgardenpi VERSION "0.20.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(RASPBIAN_ROOTFS "$ENV{HOME}/raspberrypi/rootfs")
set(FILE_LOCK_PATH "/tmp/hgardenpi.pid")
set(FILE_CONFIG "./config.json")
set(BROKER_HOST "passy-raspberrypi")
set(BROKER_PORT 1883)
set(BROKER_USER "hgardepi")
set(BROKER_PASSWD "hgardepi")
set(DB_FILE "./hgardenpi.db")



include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.9.1)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(SQLiteCpp
        GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
        GIT_TAG 3.1.1)
FetchContent_MakeAvailable(SQLiteCpp)
FetchContent_Declare(SQLiteCpp
        GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
        GIT_TAG 3.1.1)
FetchContent_MakeAvailable(SQLiteCpp)
FetchContent_Declare( date_src
        GIT_REPOSITORY https://github.com/HowardHinnant/date.git
        GIT_TAG        v3.0.1  # adjust tag/branch/commit as needed
        )
FetchContent_MakeAvailable(date_src)



configure_file(config.h.in ../src/config.h)
#include_directories("3thparty/include/date")
include_directories("${RASPBIAN_ROOTFS}/usr/local/include")
include_directories("${RASPBIAN_ROOTFS}/usr/include")
link_directories("${RASPBIAN_ROOTFS}/usr/local/lib")
link_directories("${RASPBIAN_ROOTFS}/usr/lib")

add_executable(hgardenpi
    src/clients/mqttclientmosquitto.cpp
    src/clients/mqttclientmosquitto.hpp
    src/clients/mqttclient.hpp
    src/factories/device.hpp 
    src/factories/deviceconcrete.cpp 
    src/factories/deviceconcrete.hpp
    src/factories/factory.hpp 
    src/factories/factoryconcrete.cpp
    src/factories/factoryconcrete.hpp
    src/factories/system.hpp 
    src/factories/systemconcrete.cpp
    src/factories/systemconcrete.hpp
    src/interfaces/loggable.hpp
    src/interfaces/object.hpp
    src/interfaces/releasable.hpp
    src/interfaces/singleton.hpp
    src/interfaces/startable.hpp
    src/daos/dao.hpp
    src/pods/configinfo.cpp
    src/pods/configinfo.hpp
    src/pods/deviceinfo.hpp
    src/services/configservice.cpp
    src/services/configservice.hpp
    src/services/deviceservice.cpp
    src/services/deviceservice.hpp
    src/services/lockservice.hpp
    src/services/lockserviceconcrete.cpp
    src/services/lockserviceconcrete.hpp
    src/services/logservice.hpp
    src/services/logserviceconcrete.cpp
    src/services/logserviceconcrete.hpp
    src/services/scheduler.hpp
    src/services/schedulerconcrete.cpp
    src/services/schedulerconcrete.hpp
    src/utilities/databaseutils.cpp
    src/utilities/databaseutils.hpp
    src/utilities/stringutils.cpp
    src/utilities/stringutils.hpp
    src/constants.cpp
    src/constants.hpp
    src/engine.cpp
    src/engine.hpp
    src/main.cpp


        src/pods/station.hpp)

target_link_libraries(hgardenpi
                        wiringPi
                        wiringPiDev
                        pthread
                        mosquitto
                        SQLiteCpp
                        sqlite3
                        dl
                        nlohmann_json::nlohmann_json
                        date::date
                        )