add_library(capra_externals STATIC
        external/imgui/backends/imgui_impl_glfw.h
        external/imgui/backends/imgui_impl_opengl3.h
        external/imgui/backends/imgui_impl_opengl3_loader.h
        external/imgui/imconfig.h
        external/imgui/imgui.h
        external/imgui/imgui_internal.h
        external/imgui/imstb_rectpack.h
        external/imgui/imstb_textedit.h
        external/imgui/imstb_truetype.h

        external/implot/implot.h
        external/implot/implot_internal.h

        external/pcg-cpp/include/pcg_extras.hpp
        external/pcg-cpp/include/pcg_random.hpp
        external/pcg-cpp/include/pcg_uint128.hpp

        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp

        external/implot/implot.cpp
        external/implot/implot_demo.cpp
        external/implot/implot_items.cpp)

add_subdirectory(external/fmt)

add_subdirectory(external/glad/cmake)
glad_add_library(glad_gl_core_mx_33 REPRODUCIBLE MX API gl:core=3.3)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

add_subdirectory(external/glm)

set(LIBNYQUIST_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
add_subdirectory(external/libnyquist)

set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_NO_CONFIG_UTIL ON CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_CONFIG OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_HRTF_DATA OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/openal-soft)

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
add_subdirectory(external/spdlog)

#set(UUID_SYSTEM_GENERATOR ON CACHE BOOL "" FORCE)
add_subdirectory(external/stduuid)

target_include_directories(capra_externals PUBLIC
        external/imgui/backends
        external/imgui
        external/implot
        external/pcg-cpp/include)

target_link_libraries(capra_externals PUBLIC
        fmt::fmt
        glad_gl_core_mx_33
        glfw
        glm::glm
        libnyquist
        OpenAL::OpenAL
        spdlog::spdlog_header_only
        stduuid)