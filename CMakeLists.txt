cmake_minimum_required(VERSION 3.16)

PROJECT(hi-happy-garden VERSION "0.52.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-fno-rtti)
add_compile_options(-fexceptions)

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

#at the moment not needed
#message(STATUS " start date download")
#FetchContent_Declare(date_src
#        GIT_REPOSITORY https://github.com/HowardHinnant/date.git
#        GIT_TAG v3.0.1
#        )
#FetchContent_MakeAvailable(date_src)

configure_file(config.h.in ../app/src/inc/config.h)

add_executable(${PROJECT_NAME} app/src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        pthread
        dl
        nlohmann_json::nlohmann_json
        )
