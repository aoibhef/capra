cmake_minimum_required(VERSION 3.12...3.23)
project(capra VERSION 0.1
        DESCRIPTION "WIP"
        LANGUAGES CXX C)

option(CAPRA_BUILD_EXAMPLES "Build example programs" ON)

if(MSVC)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    set(CMAKE_CXX_STANDARD 20)
endif()

add_library(capra STATIC
        include/capra/capra.hpp
        src/capra/capra.cpp)

add_subdirectory(external/fmt)

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
add_subdirectory(external/spdlog)

target_include_directories(capra PUBLIC
        include)

target_link_libraries(capra PUBLIC
        fmt::fmt
        spdlog::spdlog)   

if(CAPRA_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()