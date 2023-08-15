cmake_minimum_required(VERSION 3.16...3.24)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

PROJECT(hhg-intf LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
add_compile_options(-fno-rtti)
add_compile_options(-fexceptions)

include_directories(inc)
include_directories(../osal/inc)
include_directories(../osal/src/unix)

file(GLOB_RECURSE HI_HAPPY_GARDEN_INTERFACES_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HI_HAPPY_GARDEN_INTERFACES_INCLUDES CONFIGURE_DEPENDS "inc/*.hpp" "inc/*.h")

add_library(${PROJECT_NAME} STATIC ${HI_HAPPY_GARDEN_INTERFACES_SOURCES} ${HI_HAPPY_GARDEN_INTERFACES_INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
