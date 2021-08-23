cmake_minimum_required(VERSION 3.16)

PROJECT(hgardenpi VERSION "0.51.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(FILE_LOCK_PATH "/tmp/hgardenpi.pid")
set(FILE_CONFIG "./config.json")
set(BROKER_HOST "localhost")
set(BROKER_PORT 1883)
set(BROKER_USER "hgardenpi")
set(BROKER_PASSWD "hgardenpi")
set(DB_FILE "./hgardenpi.db")

#test macro
set(TEST 1)

#date param
#set(BUILD_TZ_LIB ON)


include(FetchContent)

message(STATUS " start nlohmann_json download")
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.10.0)
FetchContent_GetProperties(json)
if (NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

message(STATUS " start SQLiteCpp download")
FetchContent_Declare(SQLiteCpp
        GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
        GIT_TAG 3.1.1)
FetchContent_MakeAvailable(SQLiteCpp)

#at the moment not needed
#message(STATUS " start date download")
#FetchContent_Declare(date_src
#        GIT_REPOSITORY https://github.com/HowardHinnant/date.git
#        GIT_TAG v3.0.1
#        )
#FetchContent_MakeAvailable(date_src)

#add_subdirectory (3thparty/happy-gardenpi-protocol)
configure_file(config.h.in ../src/config.h)
include_directories("3thparty/happy-gardenpi-protocol/include")
include_directories("${RASPBIAN_ROOTFS}/usr/local/include")
include_directories("${RASPBIAN_ROOTFS}/usr/include")
link_directories("${RASPBIAN_ROOTFS}/usr/local/lib")
link_directories("${RASPBIAN_ROOTFS}/usr/lib")

add_executable(hgardenpi
        3thparty/happy-gardenpi-protocol/src/protocol.cpp
        src/clients/mqttclientmosquitto.cpp
        src/clients/mqttclientmosquitto.hpp
        src/clients/mqttclient.hpp
        src/components/buttonconcrete.cpp
        src/components/buttonconcrete.hpp
        src/components/button.hpp
        src/components/display.hpp
        src/components/lcd1602.cpp
        src/components/lcd1602.hpp
        src/components/relaymodule.hpp
        src/components/relaymodule4channel.cpp
        src/components/relaymodule4channel.hpp
        src/factories/device.hpp
        src/factories/deviceconcrete.cpp
        src/factories/deviceconcrete.hpp
        src/factories/factory.hpp
        src/factories/factoryconcrete.cpp
        src/factories/factoryconcrete.hpp
        src/factories/system.hpp
        src/factories/systemconcrete.cpp
        src/factories/systemconcrete.hpp
        src/interfaces/closeable.hpp
        src/interfaces/initializable.hpp
        src/interfaces/loggable.hpp
        src/interfaces/object.hpp
        src/interfaces/singleton.hpp
        src/interfaces/startable.hpp
        src/interfaces/stoppable.hpp
        src/daos/aggregationdao.cpp
        src/daos/aggregationdao.hpp
        src/daos/dao.hpp
        src/daos/stationdao.cpp
        src/daos/stationdao.hpp
        src/pods/aggregation.cpp
        src/pods/aggregation.hpp
        src/pods/configinfo.cpp
        src/pods/configinfo.hpp
        src/pods/deviceinfo.hpp
        src/pods/station.hpp
        src/services/configservice.hpp
        src/services/configserviceconcrete.cpp
        src/services/configserviceconcrete.hpp
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
        src/constants.hpp
        src/engine.cpp
        src/engine.hpp
        src/main.cpp
        src/threadengine.cpp
        src/threadengine.hpp)

target_link_libraries(hgardenpi
        wiringPi
        wiringPiDev
        pthread
        mosquitto
        SQLiteCpp
        sqlite3
        dl
        nlohmann_json::nlohmann_json
#        date::date
        )