cmake_minimum_required(VERSION 3.16...3.24)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()


PROJECT(hi-happy-garden-app VERSION "0.50.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(SCHEDULES_SIZE 2)
set(ZONES_SIZE 4)

set(FSM_MAIN_SLEEP 100)
set(FSM_ERROR_SLEEP 1000)
set(FSM_ERROR_MAX 4)

add_definitions(-DINCLUDE_HHG_CONFIG)

add_compile_options("$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fno-rtti>")
add_compile_options("$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fexceptions>")

file(GLOB_RECURSE HI_HAPPY_GARDEN_APP_INCLUDES CONFIGURE_DEPENDS "hhg-app/inc/*.hpp" "hhg-app/inc/*.h")
file(GLOB_RECURSE HI_HAPPY_GARDEN_APP_SOURCES CONFIGURE_DEPENDS "hhg-app/src/*.cpp" "hhg-app/src/*.c")
file(GLOB_RECURSE HI_HAPPY_GARDEN_UTILS_INCLUDES CONFIGURE_DEPENDS "hhg-utils/src/*.cpp" "hhg-utils/src/*.c")
file(GLOB_RECURSE HI_HAPPY_GARDEN_UTILS_SOURCES CONFIGURE_DEPENDS "hhg-utils/inc/*.hpp" "hhg-utils/inc/*.h")
if(UNIX)
    file(GLOB_RECURSE HI_HAPPY_GARDEN_DRIVER_INCLUDES CONFIGURE_DEPENDS "hhg-driver/linux/*.hpp" "hhg-driver/linux/*.h")
    file(GLOB_RECURSE HI_HAPPY_GARDEN_DRIVER_SOURCES CONFIGURE_DEPENDS "hhg-driver/platform/linux/*.cpp" "hhg-driver/platform/linux/*.c")
elseif (PLATFORM EQUAL pico)

    add_definitions(-DINCLUDE_HHG_PARSER_CONFIG)

    file(GLOB_RECURSE HI_HAPPY_GARDEN_DRIVER_INCLUDES CONFIGURE_DEPENDS "hhg-driver/inc/*.hpp" "hhg-driver/inc/*.h")
    file(GLOB_RECURSE HI_HAPPY_GARDEN_DRIVER_SOURCES CONFIGURE_DEPENDS "hhg-driver/platform/pico/*.cpp" "hhg-driver/platform/pico/*.c")
else ()
    error("No platform recognized")
endif ()

message(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

configure_file(hhg-config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/hhg-config/hhg-config.h)

include_directories(hhg-app/inc)
include_directories(hhg-drivers/inc)
include_directories(hhg-intf/inc)
include_directories(hhg-utils/inc)
include_directories(hhg-config)
include_directories(inc)
include_directories(config)


include_directories(osal/inc)
include_directories(osal/src/osal_sys)
include_directories(osal/src/unix)
add_subdirectory(osal)

include_directories(hhg-parser/inc)
add_subdirectory(hhg-parser)

include_directories(cJSON)
add_subdirectory(cJSON)


add_executable(${PROJECT_NAME}
                ${HI_HAPPY_GARDEN_APP_INCLUDES} ${HI_HAPPY_GARDEN_APP_SOURCES}
                ${HI_HAPPY_GARDEN_DRIVER_INCLUDES} ${HI_HAPPY_GARDEN_DRIVER_SOURCES}
                ${HI_HAPPY_GARDEN_UTILS_INCLUDES} ${HI_HAPPY_GARDEN_UTILS_SOURCES}
)
target_link_libraries(${PROJECT_NAME}
        cjson
        osal
        hi-happy-garden-parser
)