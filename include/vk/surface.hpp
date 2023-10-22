#pragma once
#include <vulkan/vulkan.h>

#include <vk/instance.hpp>

#include <memory>

namespace vk {
    class Surface;
}

class vk::Surface {
private:
    std::shared_ptr<vk::Instance> instance;
public:
    VkSurfaceKHR surface;

    Surface(std::shared_ptr<vk::Instance> instance, GLFWwindow* window);
    ~Surface();
};