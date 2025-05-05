cmake_minimum_required(VERSION 3.20.0)
project(OpenGL_Raytracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

# get all .cpp / .hpp files from src
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.hpp
    src/*.cpp
)



add_executable(OpenGL_Raytracer 
    main.cpp
    vendor/glad/src/glad.c

    # src files
    ${SRC_FILES}

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
add_subdirectory(vendor/assimp)

target_link_libraries(OpenGL_Raytracer OpenGL::GL glfw assimp)

# copy ressources folder to build
add_custom_target(copy_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/ressources/"
    "${CMAKE_BINARY_DIR}/ressources/"
    COMMENT "Copying Data"
)
add_dependencies(OpenGL_Raytracer copy_data)
