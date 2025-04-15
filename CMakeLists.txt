cmake_minimum_required(VERSION 3.20.0)
project(OpenGL_Raytracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

find_package(OpenGL REQUIRED)


add_executable(OpenGL_Raytracer 
    main.cpp
    vendor/glad/src/glad.c

    # ImGui core
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/imgui_demo.cpp         # optional

    # ImGui backends
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    )

target_include_directories(OpenGL_Raytracer SYSTEM PRIVATE "vendor/glm" "vendor/glad/include" "vendor/glfw/include" "vendor/imgui" "vendor/imgui/backends")
target_include_directories(OpenGL_Raytracer PUBLIC "src")

add_subdirectory(vendor/glfw)

target_link_libraries(OpenGL_Raytracer OpenGL::GL glfw)



include(CTest)
enable_testing()