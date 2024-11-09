file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/vk/*.hpp")

add_library(
    vk
    instance.cpp
    surface.cpp
    device.cpp
    queues.cpp
    shader.cpp
    swapChain.cpp
    ${HEADER_LIST})

target_include_directories(vk PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_compile_features(vk PUBLIC cxx_std_20)
target_compile_options(vk PUBLIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
target_link_libraries(vk PUBLIC Vulkan::Vulkan glfw)


source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
