#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace vk {
    class Instance;
};

class vk::Instance {
private:
    bool enableValidationLayers;
    VkDebugUtilsMessengerEXT debugMessenger;

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
    bool checkExtensionSupport(const std::vector<const char*>& requiredExtensions, const std::vector<VkExtensionProperties>& aveilableExtensions);
    VkResult CreateDebugUtilsMessengerExt(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    std::vector<const char*> getRequiredExtensions();

public:
    VkInstance instance;

    Instance(GLFWwindow* window, const std::vector<const char*>& validationLayers);
    ~Instance();
};