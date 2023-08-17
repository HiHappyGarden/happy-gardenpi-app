cmake_minimum_required(VERSION 3.16...3.24)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

PROJECT(hhg-app LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
add_compile_options("$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fno-rtti>")
add_compile_options("$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fexceptions>")

include(FetchContent)
# cJSON
FetchContent_Declare(
    cjson
    URL https://github.com/DaveGamble/cJSON/archive/refs/tags/v1.7.16.tar.gz
)
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(ENABLE_CJSON_TEST OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(cjson)


include_directories(inc)
include_directories("${CMAKE_BINARY_DIR}/_deps/cjson-src")

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

include_directories(../osal/inc)
include_directories(../interfaces/inc)

if(LINUX)
    include_directories(../osal/src/unix)
else()
    message(ERROR " -- Platform not suppoted")
endif()

file(GLOB_RECURSE HI_HAPPY_GARDEN_APP_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HI_HAPPY_GARDEN_APP_INCLUDES CONFIGURE_DEPENDS "inc/*.hpp" "inc/*.h")


add_library(${PROJECT_NAME} STATIC ${HI_HAPPY_GARDEN_APP_SOURCES} ${HI_HAPPY_GARDEN_APP_INCLUDES})
target_link_libraries(${PROJECT_NAME} PRIVATE cjson)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
