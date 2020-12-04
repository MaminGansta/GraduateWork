cmake_minimum_required(VERSION 3.12)
project(ImGui VERSION 0.0.0)

set(IMGUI_SOURCES
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_opengl3.cpp
    imgui_impl_sdl.cpp
    imgui_widgets.cpp
)

set(IMGUI_HEADER
    imconfig.h
    imgui.h
    imgui_impl_opengl3.h
    imgui_impl_sdl.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

target_include_directories(
    imgui
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../GLEW/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../SDL2/include
)