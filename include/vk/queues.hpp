#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vk/surface.hpp>

#include <optional>
#include <memory>

namespace vk {
	class QueueFamilyIndices;
}

class vk::QueueFamilyIndices {
public:
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	QueueFamilyIndices() = default;
	QueueFamilyIndices(VkPhysicalDevice device, std::shared_ptr<vk::Surface> surface);
	bool isComplete();
};

