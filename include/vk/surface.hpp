#pragma once
#include <vulkan/vulkan.h>

#include <vk/instance.hpp>

namespace vk {
    class Surface;
}

class vk::Surface {
private:
    vk::Instance* instance;
public:
    VkSurfaceKHR surface;

    void init(vk::Instance& instance, GLFWwindow* window);
    ~Surface();
};