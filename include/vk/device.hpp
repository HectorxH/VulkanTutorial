#pragma once
#include <vulkan/vulkan.h>

#include <memory>
#include <vk/instance.hpp>
#include <vk/queues.hpp>
#include <vk/surface.hpp>

namespace vk {
class Device;
}

class vk::Device {
 private:
  std::shared_ptr<vk::Instance> instance;
  std::shared_ptr<vk::Surface> surface;

  bool isDeviceSuitable(
      VkPhysicalDevice device,
      const std::vector<const char*>& requiredExtensions
  );
  bool checkDeviceExtensionSupport(
      VkPhysicalDevice device,
      const std::vector<const char*>& requiredExtensions
  );

 public:
  VkPhysicalDevice physicalDevice;
  vk::QueueFamilyIndices indices;
  VkDevice device;
  VkQueue presentQueue;
  VkQueue graphicsQueue;

  Device(
      std::shared_ptr<vk::Instance> instance,
      std::shared_ptr<vk::Surface> surface,
      const std::vector<const char*>& requiredExtensions
  );
  ~Device();
};