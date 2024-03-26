cmake_minimum_required(VERSION 3.16...3.24)

add_definitions(-DPICO_CXX_DISABLE_ALLOCATION_OVERRIDES)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

PROJECT(hi-happy-garden-app VERSION "0.50.0" LANGUAGES CXX C ASM)
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

pico_sdk_init()

add_definitions(-DINCLUDE_HHG_CONFIG)
add_definitions(-DPICO)
add_definitions(-DOS_MEM_LAYER)
add_definitions(-DEXCLUDE_CHECK_FOR_STACK_OVERFLOW)
add_definitions(-DEXCLUDE_USE_MALLOC_FAILED_HOOK)

include_directories(cJSON)
include_directories(inc)
include_directories(osal/inc)
include_directories(osal/src/freertos)
include_directories(hhg-config/pico)
include_directories(hhg-utils/inc)
include_directories(hhg-iface/inc)
include_directories(hhg-drivers/inc)
include_directories(hhg-app/inc)
include_directories(hhg-parser/inc)

add_subdirectory(freertos)
add_subdirectory(hhg-parser)


file(GLOB_RECURSE OSAL_INCLUDES CONFIGURE_DEPENDS "osal/inc/*.hpp" "osal/src/freertos/osal_sys/*.hpp" "osal/src/freertos/config/*.h")
file(GLOB_RECURSE OSAL_SOURCES CONFIGURE_DEPENDS "osal/src/error.cpp" "osal/src/log.cpp" "osal/src/generics.cpp" "osal/src/freertos/*.cpp")

file(GLOB_RECURSE HHG_DRIVERS_INCLUDES CONFIGURE_DEPENDS "hhg-drivers/inc/hhg-driver/*.hpp" "hhg-drivers/inc/hhg-driver/*.h" "hhg-drivers/inc/pico/*.hpp" "hhg-drivers/inc/pico/*.h")
file(GLOB_RECURSE HHG_DRIVERS_SOURCES CONFIGURE_DEPENDS "hhg-drivers/platform/pico/*.cpp" "hhg-drivers/platform/pico/*.c")

file(GLOB_RECURSE HHG_APP_INCLUDES CONFIGURE_DEPENDS "hhg-app/inc/*.hpp" "hhg-app/inc/*.h")
file(GLOB_RECURSE HHG_APP_SOURCES CONFIGURE_DEPENDS "hhg-app/src/*.cpp" "hhg-app/src/*.c")

file(GLOB_RECURSE HHG_UTILS_INCLUDES CONFIGURE_DEPENDS "hhg-utils/inc/*.hpp" "hhg-utils/inc/*.h")
file(GLOB_RECURSE HHG_UTILS_SOURCES CONFIGURE_DEPENDS "hhg-utils/src/*.cpp" "hhg-utils/src/*.c")

add_executable(${PROJECT_NAME}
        src/main.cpp
        cJSON/cJSON.h cJSON/cJSON.c cJSON/cJSON_Utils.h cJSON/cJSON_Utils.c
        ${OSAL_INCLUDES} ${OSAL_SOURCES}
        ${HHG_DRIVERS_INCLUDES} ${HHG_DRIVERS_SOURCES}
        ${HHG_APP_INCLUDES} ${HHG_APP_SOURCES}
        ${HHG_UTILS_INCLUDES} ${HHG_UTILS_SOURCES}
        hhg-drivers/inc/pico/pico-gpio.hpp
        hhg-drivers/platform/pico/pico-gpio.cpp
)

target_link_libraries(${PROJECT_NAME}
        pico_stdlib
        hardware_rtc
        freertos
        hi-happy-garden-parser
)

pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_uart(${PROJECT_NAME} 1)


