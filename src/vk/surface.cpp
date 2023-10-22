#include <vk/surface.hpp>

#include <stdexcept>


void vk::Surface::init(vk::Instance& instance, GLFWwindow* window) {
    this->instance = &instance;
    if (glfwCreateWindowSurface(instance.instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

vk::Surface::~Surface() {
    vkDestroySurfaceKHR(instance->instance, surface, nullptr);
}