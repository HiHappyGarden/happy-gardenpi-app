cmake_minimum_required(VERSION 3.16...3.24)


option(PICO "Build for Rasberry Pi Pico" ON)

if (PICO)
	include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()


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
elseif (PICO)

    option(FREERTOS "Enable FreeRTOS layer on OASL" ON)

    include(FetchContent)
    message(STATUS "Start download FreeRTOS Kernel")
    FetchContent_Declare( freertos_kernel
            GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
            GIT_TAG        V11.0.1
    )

    add_library(freertos_config INTERFACE)

    target_include_directories(freertos_config SYSTEM
            INTERFACE
            src/freertos/config
    )

    target_compile_definitions(freertos_config
            INTERFACE
            projCOVERAGE_TEST=0
    )


    set( FREERTOS_HEAP "4" CACHE STRING "" FORCE)
    # Select the native compile PORT
    set( FREERTOS_PORT "GCC_RP2040" CACHE STRING "" FORCE)
    # Select the cross-compile PORT
    #        if (CMAKE_CROSSCOMPILING)
    #            set(FREERTOS_PORT "GCC_ARM_CA9" CACHE STRING "" FORCE)
    #        endif()
    include_directories(${CMAKE_BINARY_DIR}/_deps/freertos_kernel-src/include)
    include_directories(${CMAKE_BINARY_DIR}/_deps/freertos_kernel-src/portable/ThirdParty/GCC/Posix)

    set(OS_MEM_LAYER 1)
    set(PLATFORM_LIB "freertos_kernel")
    FetchContent_MakeAvailable(freertos_kernel)
    set(PLATFORM_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/config)
    include_directories(${PLATFORM_INCLUDE})
    pico_sdk_init()

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
include_directories(osal/src/freertos/osal_sys)
include_directories(osal/src/freertos)
add_subdirectory(osal)

include_directories(hhg-parser/inc)
add_subdirectory(hhg-parser)

set(BUILD_SHARED_LIBS OFF)
include_directories(cJSON)
add_subdirectory(cJSON)


add_executable(${PROJECT_NAME}
                ${HI_HAPPY_GARDEN_APP_INCLUDES} ${HI_HAPPY_GARDEN_APP_SOURCES}
                ${HI_HAPPY_GARDEN_DRIVER_INCLUDES} ${HI_HAPPY_GARDEN_DRIVER_SOURCES}
                ${HI_HAPPY_GARDEN_UTILS_INCLUDES} ${HI_HAPPY_GARDEN_UTILS_SOURCES}
)

if (PICO)

target_link_libraries(${PROJECT_NAME}
	pico_stdlib
        cjson
        osal
        hi-happy-garden-parser
)

pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})

endif()
