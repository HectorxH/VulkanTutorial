#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>
#include <vk/surface.hpp>

namespace vk {
class SwapChainSupportDetails;
class SwapChain;
}  // namespace vk

class vk::SwapChainSupportDetails {
 public:
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;

  SwapChainSupportDetails(
      VkPhysicalDevice device, std::shared_ptr<vk::Surface> surface
  );

  VkSurfaceFormatKHR chooseSwapSurfaceFormat();
  VkPresentModeKHR chooseSwapPresentMode();
  VkExtent2D chooseSwapExtent(GLFWwindow* window);
};

class vk::SwapChain {
 private:
 public:
};
