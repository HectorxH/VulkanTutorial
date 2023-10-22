#include <vk/devices.hpp>

#include <vk/queues.hpp>
#include <vk/swapChain.hpp>

#include <cstring>

vk::PhysicalDevice::PhysicalDevice(
    std::shared_ptr<vk::Instance> instance,
    std::shared_ptr<vk::Surface> surface,
    const std::vector<const char*>& requiredExtensions
) : instance(instance), surface(surface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance->instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("no physical devices with vulkan support found!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance->instance, &deviceCount, physicalDevices.data());

    for (const auto& physicalDevice : physicalDevices) {
        if (isDeviceSuitable(physicalDevice, requiredExtensions)) {
            device = physicalDevice;
            return;
        }
    }

    throw std::runtime_error("no suitable physical device found!");
}

bool vk::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    indices = vk::QueueFamilyIndices(device, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device, requiredExtensions);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty()
            && !swapChainSupport.presentModes.empty();
    }

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
        && indices.isComplete()
        && extensionsSupported
        && swapChainAdequate;
}

bool vk::PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());


    for (const char* requiredExtension : requiredExtensions) {
        bool extensionFound = false;
        for (const auto& availableExtension : availableExtensions) {
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
