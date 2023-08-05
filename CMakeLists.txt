cmake_minimum_required(VERSION 3.16...3.24)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()


PROJECT(hi-happy-garden VERSION "0.52.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
add_definitions(-DOS_NOEXCEPT=)

add_compile_options(-fno-rtti)
add_compile_options(-fexceptions)

set(OS_NOEXCEPT "")

subdirs(app)
subdirs(interfaces)
subdirs(osal)
subdirs(platforms)


configure_file(config.h.in ../app/src/inc/config.h)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
    hhg-intf
    osal
    hhg-platforms
    dl
    )
