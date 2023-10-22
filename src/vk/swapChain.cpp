#include <vk/swapChain.hpp>

#include <algorithm>
#include <limits>  

vk::SwapChainSupportDetails::SwapChainSupportDetails(VkPhysicalDevice device, std::shared_ptr<vk::Surface> surface) {
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface->surface, &capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface->surface, &formatCount, nullptr);

	if (formatCount != 0) {
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface->surface, &formatCount, formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface->surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface->surface, &presentModeCount, presentModes.data());
	}
}

VkSurfaceFormatKHR vk::SwapChainSupportDetails::chooseSwapSurfaceFormat() {
	for (const auto& format : formats) {
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB
			&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return format;
		}
	}

	return formats[0];
}

VkPresentModeKHR vk::SwapChainSupportDetails::chooseSwapPresentMode() {
	for (const auto& presentMode : presentModes) {
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return presentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D vk::SwapChainSupportDetails::chooseSwapExtent(GLFWwindow* window) {
	if (capabilities.currentExtent.width != std::numeric_limits<unsigned int>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			std::clamp(
				static_cast<uint32_t>(width),
				capabilities.minImageExtent.width,
				capabilities.maxImageExtent.width
			),
			std::clamp(
				static_cast<uint32_t>(height),
				capabilities.minImageExtent.height,
				capabilities.maxImageExtent.height
			),
		};

		return actualExtent;
	}
}