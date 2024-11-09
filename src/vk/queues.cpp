#include <cstdint>
#include <stdexcept>
#include <vector>
#include <vk/queues.hpp>

bool vk::QueueFamilyIndices::isComplete() {
  return graphicsFamily.has_value() && presentFamily.has_value();
}

vk::QueueFamilyIndices::QueueFamilyIndices(
    VkPhysicalDevice device, std::shared_ptr<vk::Surface> surface
) {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
      device, &queueFamilyCount, queueFamilies.data()
  );

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        device, i, surface->surface, &presentSupport
    );
    if (presentSupport) {
      presentFamily = i;
    }

    if (isComplete()) {
      break;
    }

    i++;
  }
}