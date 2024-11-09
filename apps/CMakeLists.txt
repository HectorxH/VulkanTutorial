add_executable(triangle triangle.cpp)
target_compile_features(triangle PRIVATE cxx_std_20)
target_compile_options(triangle PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)

target_link_libraries(
    triangle PRIVATE
    vk)

add_dependencies(triangle Shaders)
