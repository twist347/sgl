set(T 01_window)

add_executable(${T} main.cpp)
target_link_libraries(${T} sgl glad)
target_compile_options(${T} PRIVATE -Wall -Wextra -Wpedantic)
