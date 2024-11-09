#include <stdexcept>
#include <vk/surface.hpp>

vk::Surface::Surface(std::shared_ptr<vk::Instance> instance, GLFWwindow *window)
    : instance(instance) {
  if (glfwCreateWindowSurface(instance->instance, window, nullptr, &surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
}

vk::Surface::~Surface() {
  vkDestroySurfaceKHR(instance->instance, surface, nullptr);
}