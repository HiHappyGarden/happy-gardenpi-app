cmake_minimum_required(VERSION 3.16...3.24)

add_definitions(-DPICO_CXX_DISABLE_ALLOCATION_OVERRIDES)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
set(PICO_BOARD pico_w)

PROJECT(hi-happy-garden VERSION "0.60.0" LANGUAGES CXX C ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(DIVISOR "|")
set(SCHEDULES_SIZE 2)
set(ZONES_SIZE 4)
set(ADMIN_USER "admin")
set(ADMIN_PASSWD "admin")
set(USER "user")
set(PASSWD "user")
set(WIFI_DISABLE 1)
set(WIFI_CONNECTION_TIMEOUT 10000)
set(NTP_MSG_LEN 48)
set(NTP_PORT 123)
set(NTP_SERVER "0.it.pool.ntp.org")
set(DAYLIGHT_SAVING_TIME_START_MONTH 2)
set(DAYLIGHT_SAVING_TIME_START_DAY 31)
set(DAYLIGHT_SAVING_TIME_START_HOUR 2)
set(DAYLIGHT_SAVING_TIME_END_MONTH 9)
set(DAYLIGHT_SAVING_TIME_END_DAY 31)
set(DAYLIGHT_SAVING_TIME_END_HOUR 3)

pico_sdk_init()

add_definitions(-DINCLUDE_HHG_CONFIG)
add_definitions(-DINCLUDE_OSAL_CONFIG)
add_definitions(-DPICO)
add_definitions(-DEXCLUDE_CHECK_FOR_STACK_OVERFLOW)
add_definitions(-DEXCLUDE_USE_MALLOC_FAILED_HOOK)
add_definitions(-DNO_SYS=0)
add_definitions(-DCYW43_LWIP=1)
add_definitions(-DCYW43_HOST_NAME=\"${PROJECT_NAME}\")



include_directories(cJSON)
include_directories(inc)
include_directories(osal/inc)
include_directories(osal/src/freertos)
include_directories(hhg-config/pico)
include_directories(hhg-utils/inc)
include_directories(hhg-iface/inc)
include_directories(hhg-driver/inc)
include_directories(hhg-app/inc)
include_directories(hhg-parser/inc)
include_directories(FreeRTOS-Kernel/include)
include_directories(FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/include)
include_directories(md5)

#add_subdirectory(freertos)
add_subdirectory(hhg-parser)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/hhg-config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/hhg-config/pico/hhg-config.h)

file(GLOB_RECURSE OSAL_INCLUDES CONFIGURE_DEPENDS "osal/inc/*.hpp" "osal/src/freertos/osal_sys/*.hpp" "osal/src/freertos/config/*.h")
file(GLOB_RECURSE OSAL_SOURCES CONFIGURE_DEPENDS "osal/src/error.cpp" "osal/src/log.cpp" "osal/src/generics.cpp" "osal/src/freertos/*.cpp")

file(GLOB_RECURSE HHG_DRIVERS_INCLUDES CONFIGURE_DEPENDS "hhg-driver/inc/hhg-driver/lcd.hpp" "hhg-driver/inc/hhg-driver/*.hpp" "hhg-driver/inc/hhg-driver/*.h" "hhg-driver/inc/pico/*.hpp" "hhg-driver/inc/pico/*.h")
file(GLOB_RECURSE HHG_DRIVERS_SOURCES CONFIGURE_DEPENDS "hhg-driver/platform/lcd.cpp" "hhg-driver/platform/pico/*.cpp" "hhg-driver/platform/pico/*.c")

file(GLOB_RECURSE HHG_APP_INCLUDES CONFIGURE_DEPENDS "hhg-app/inc/*.hpp" "hhg-app/inc/*.h")
file(GLOB_RECURSE HHG_APP_SOURCES CONFIGURE_DEPENDS "hhg-app/src/*.cpp" "hhg-app/src/*.c")

file(GLOB_RECURSE HHG_UTILS_INCLUDES CONFIGURE_DEPENDS "hhg-utils/inc/*.hpp" "hhg-utils/inc/*.h")
file(GLOB_RECURSE HHG_UTILS_SOURCES CONFIGURE_DEPENDS "hhg-utils/src/*.cpp" "hhg-utils/src/*.c")

file(GLOB_RECURSE FREERTOS_KERNEL_INCLUDES CONFIGURE_DEPENDS "FreeRTOS-Kernel/include/*.h" "FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/include/*.h")
file(GLOB_RECURSE FREERTOS_KERNEL_SOURCES CONFIGURE_DEPENDS "FreeRTOS-Kernel/croutine.c" "FreeRTOS-Kernel/event_groups.c" "FreeRTOS-Kernel/list.c" "FreeRTOS-Kernel/queue.c" "FreeRTOS-Kernel/stream_buffer.c" "FreeRTOS-Kernel/tasks.c" "FreeRTOS-Kernel/timers.c" "FreeRTOS-Kernel/portable/MemMang/heap_2.c" "FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/*.c")

message(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

add_executable(${PROJECT_NAME}
        src/main.cpp
        cJSON/cJSON.h cJSON/cJSON.c cJSON/cJSON_Utils.h cJSON/cJSON_Utils.c
        md5/md5.h md5/md5.cpp
        ${OSAL_INCLUDES} ${OSAL_SOURCES}
        ${HHG_DRIVERS_INCLUDES} ${HHG_DRIVERS_SOURCES}
        ${HHG_APP_INCLUDES} ${HHG_APP_SOURCES}
        ${HHG_UTILS_INCLUDES} ${HHG_UTILS_SOURCES}
        ${FREERTOS_KERNEL_INCLUDES} ${FREERTOS_KERNEL_SOURCES}
)


target_link_libraries(${PROJECT_NAME}
        pico_cyw43_arch_lwip_sys_freertos
        pico_stdlib
        pico_lwip_iperf
        #pico_lwip_mbedtls
        pico_multicore
        pico_unique_id
        hardware_rtc
        hardware_pio
        hardware_uart
        hardware_flash
        hardware_sync
        hardware_gpio
        hardware_i2c
        hardware_pwm
        hardware_exception
        hi-happy-garden-parser
)

pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

