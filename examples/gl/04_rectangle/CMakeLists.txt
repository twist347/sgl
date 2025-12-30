set(T 04_rectangle)

add_executable(${T} main.cpp)
target_link_libraries(${T} sgl glad)
target_compile_options(${T} PRIVATE -Wall -Wextra -Wpedantic)

add_custom_target(copy_shaders_${T} ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_CURRENT_BINARY_DIR}/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "${CMAKE_CURRENT_BINARY_DIR}/shaders"
)

add_dependencies(${T} copy_shaders_${T})
