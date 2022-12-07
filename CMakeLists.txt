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

include(include/CMakeLists.txt)
include(src/CMakeLists.txt)
add_library(capra STATIC ${CAPRA_INCLUDE_FILES} ${CAPRA_SRC_FILES})

target_include_directories(capra PUBLIC include)

include(external/CMakeLists.txt)
target_link_libraries(capra PUBLIC capra_externals)

if(CAPRA_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()