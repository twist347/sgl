set(T 07_texture)

add_executable(${T} main.cpp)
target_link_libraries(${T} sgl)

add_custom_target(copy_shaders_${T} ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_CURRENT_BINARY_DIR}/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "${CMAKE_CURRENT_BINARY_DIR}/shaders"
)

add_custom_target(copy_textures_${T} ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_CURRENT_BINARY_DIR}/textures"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/textures"
        "${CMAKE_CURRENT_BINARY_DIR}/textures"
)

add_dependencies(${T} copy_shaders_${T})
add_dependencies(${T} copy_textures_${T})
