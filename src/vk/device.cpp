#include <cstring>
#include <set>
#include <vk/device.hpp>
#include <vk/queues.hpp>
#include <vk/swapChain.hpp>

vk::Device::Device(
    std::shared_ptr<vk::Instance> instance,
    std::shared_ptr<vk::Surface> surface,
    const std::vector<const char *> &requiredExtensions
)
    : instance(instance), surface(surface) {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance->instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw std::runtime_error("no physical devices with vulkan support found");
  }

  std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
  vkEnumeratePhysicalDevices(
      instance->instance, &deviceCount, physicalDevices.data()
  );

  bool found_suitable_device = false;
  for (const auto &candidateDevice : physicalDevices) {
    if (isDeviceSuitable(candidateDevice, requiredExtensions)) {
      physicalDevice = candidateDevice;
      found_suitable_device = true;
      break;
    }
  }

  if (!found_suitable_device) {
    throw std::runtime_error("no suitable physical device found");
  }

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = {
      indices.graphicsFamily.value(),
      indices.presentFamily.value(),
  };

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(requiredExtensions.size());
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();

  if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device)
      != VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device");
  }

  vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
  vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

vk::Device::~Device() { vkDestroyDevice(device, nullptr); }

bool vk::Device::isDeviceSuitable(
    VkPhysicalDevice device, const std::vector<const char *> &requiredExtensions
) {
  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);

  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

  indices = vk::QueueFamilyIndices(device, surface);

  bool extensionsSupported =
      checkDeviceExtensionSupport(device, requiredExtensions);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport(device, surface);
    swapChainAdequate = !swapChainSupport.formats.empty()
                        && !swapChainSupport.presentModes.empty();
  }

  return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
         && indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool vk::Device::checkDeviceExtensionSupport(
    VkPhysicalDevice device, const std::vector<const char *> &requiredExtensions
) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(
      device, nullptr, &extensionCount, nullptr
  );

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(
      device, nullptr, &extensionCount, availableExtensions.data()
  );

  for (const char *requiredExtension : requiredExtensions) {
    bool extensionFound = false;
    for (const auto &availableExtension : availableExtensions) {
      if (strcmp(requiredExtension, availableExtension.extensionName) == 0) {
        extensionFound = true;
        break;
      }
    }

    if (!extensionFound) {
      return false;
    }
  }

  return true;
}
