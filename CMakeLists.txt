cmake_minimum_required(VERSION 3.20)

set(T sgl)

project(${T})

find_package(OpenGL REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)

set(GLFW_DIR ${EXTERNAL_DIR}/glfw-3.4)
set(GLAD_DIR ${EXTERNAL_DIR}/glad)
set(STB_IMAGE_DIR ${EXTERNAL_DIR}/stb_image)
set(TL_DIR ${EXTERNAL_DIR}/tl)
set(GLM_DIR ${EXTERNAL_DIR}/glm-1.0.2)
set(FMT_DIR ${EXTERNAL_DIR}/fmt-12.1.0)

add_subdirectory(${GLFW_DIR} EXCLUDE_FROM_ALL)
add_subdirectory(${GLAD_DIR} EXCLUDE_FROM_ALL)
add_subdirectory(${GLM_DIR} EXCLUDE_FROM_ALL)
add_subdirectory(${FMT_DIR} EXCLUDE_FROM_ALL)

add_library(
        ${T} STATIC
        src/sgl_window.cpp
        src/sgl_render.cpp
        src/sgl_log.cpp
        src/sgl_shader.cpp
        src/sgl_type.cpp
        src/sgl_vertex_buffer.cpp
        src/sgl_element_buffer.cpp
        src/sgl_vertex_array.cpp
        src/sgl_info.cpp
        src/sgl_backend.cpp
        src/sgl_file.cpp
        src/sgl_texture.cpp
        src/sgl_time.cpp
        src/sgl_input.cpp
        src/sgl_gl_info.cpp
        src/sgl_camera.cpp
        src/sgl_stb_image_impl.cpp
)

target_compile_features(${T} PUBLIC cxx_std_20)
target_compile_definitions(${T} PRIVATE GLFW_INCLUDE_NONE)
target_include_directories(
        ${T}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${TL_DIR}
        PRIVATE
        ${STB_IMAGE_DIR}
)
target_link_libraries(${T} PUBLIC glm::glm PRIVATE glad glfw fmt::fmt OpenGL::GL)
target_compile_options(${T} PRIVATE -Wall -Wextra -Wpedantic)

add_subdirectory(${EXAMPLES_DIR})
