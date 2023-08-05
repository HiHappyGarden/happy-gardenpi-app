cmake_minimum_required(VERSION 3.16...3.24)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

PROJECT(hhg-platforms LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()


if(LINUX)
    message(STATUS " -- Platform Linux")
    include_directories(generic/linux/inc)

    file(GLOB_RECURSE HI_HAPPY_GARDEN_PLATFORMS_SOURCES CONFIGURE_DEPENDS "linux/src/*.cpp" "linux/src/*.c")
    file(GLOB_RECURSE HI_HAPPY_GARDEN_PLATFORMS_INCLUDE CONFIGURE_DEPENDS "linux/inc/hhg-platform/*.hpp" "inc/hhg-platform/*.h")

    configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/linux/inc/hhg-platform/config.h)
else()
    message(ERROR " -- Platform not suppoted")
endif()








add_library(${PROJECT_NAME} STATIC ${HI_HAPPY_GARDEN_PLATFORMS_SOURCES} ${HI_HAPPY_GARDEN_PLATFORMS_INCLUDE})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}generic//inc)
