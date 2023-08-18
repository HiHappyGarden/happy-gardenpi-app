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
add_compile_options("$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fno-rtti>")
add_compile_options("$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fexceptions>")

set(LCD_MSG_SIZE 34)
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    set(FILE_PATH_DATA "data.json")
    set(FILE_PATH_CONF "conf.json")
endif()
configure_file(config.h.in "${PROJECT_SOURCE_DIR}/inc/config.h")


subdirs(app)
subdirs(interfaces)
subdirs(osal)
subdirs(platforms)




include_directories(inc)

file(GLOB_RECURSE HI_HAPPY_GARDEN_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HI_HAPPY_GARDEN_INCLUDES CONFIGURE_DEPENDS "inc/*.hpp" "inc/*.h")
file(GLOB_RECURSE HI_HAPPY_GARDEN_TEST CONFIGURE_DEPENDS "test/*.cpp" "test/*.hpp")

message(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

add_executable(${PROJECT_NAME} ${HI_HAPPY_GARDEN_SOURCES} ${HI_HAPPY_GARDEN_INCLUDES})

target_link_libraries(${PROJECT_NAME}
    hhg-app
    hhg-intf
    osal
    hhg-platforms
    dl
    )

enable_testing()

add_executable("${PROJECT_NAME}_test" ${HI_HAPPY_GARDEN_TEST})

target_link_libraries("${PROJECT_NAME}_test"
                        osal
                        hhg-app
                        hhg-platforms
                        dl
                        GTest::gtest
                        GTest::gtest_main
                        )

add_test("${PROJECT_NAME}_gtest" "${PROJECT_NAME}_test")