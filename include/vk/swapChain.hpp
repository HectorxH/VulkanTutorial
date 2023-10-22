#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vk/surface.hpp>
#include <vector>
#include <memory>

namespace vk {
	class SwapChainSupportDetails;
	class SwapChain;
}

class vk::SwapChainSupportDetails {
public:
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;

	SwapChainSupportDetails(VkPhysicalDevice device, std::shared_ptr<vk::Surface> surface);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat();
	VkPresentModeKHR chooseSwapPresentMode();
	VkExtent2D chooseSwapExtent(GLFWwindow* window);
};

class vk::SwapChain {
private:
public:
};



