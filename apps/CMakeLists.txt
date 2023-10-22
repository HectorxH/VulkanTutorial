add_executable(triangle triangle.cpp)
target_compile_features(triangle PRIVATE cxx_std_20)

target_link_libraries(
    triangle PRIVATE
    vk)

add_dependencies(triangle Shaders)