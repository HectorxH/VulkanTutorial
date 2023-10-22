#pragma once
#include <vulkan/vulkan.h>

#include <vk/instance.hpp>
#include <vk/surface.hpp>
#include <vk/queues.hpp>

#include <memory>

namespace vk {
    class PhysicalDevice;
    class VirtualDevice;
}

class vk::PhysicalDevice {
private:
    std::shared_ptr<vk::Instance> instance;
    std::shared_ptr<vk::Surface> surface;

    bool isDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions);

public:
    VkPhysicalDevice device;
    vk::QueueFamilyIndices indices;

    PhysicalDevice(std::shared_ptr<vk::Instance> instance, std::shared_ptr<vk::Surface> surface, const std::vector<const char*>& requiredExtensions);
};