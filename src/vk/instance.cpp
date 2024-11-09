#include <GLFW/glfw3.h>

#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>
#include <vk/instance.hpp>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData
) {
  if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    std::cerr << std::format("validation layer: {}\n", pCallbackData->pMessage);
  }

  return VK_FALSE;
}

vk::Instance::Instance(
    GLFWwindow *window, const std::vector<const char *> &validationLayers
)
    : enableValidationLayers(!validationLayers.empty()) {
  if (enableValidationLayers
      && !checkValidationLayerSupport(validationLayers)) {
    throw std::runtime_error("validation layers requested, but not available");
  }

  std::vector<const char *> requiredExtensions = getRequiredExtensions();
  if (enableValidationLayers) {
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(
      nullptr, &extensionCount, availableExtensions.data()
  );

  if (!checkExtensionSupport(requiredExtensions, availableExtensions)) {
    throw std::runtime_error("not all required extensions are available");
  }

  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(requiredExtensions.size());
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();
  createInfo.enabledLayerCount = 0;

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext =
        static_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&debugCreateInfo);
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance");
  }

  if (enableValidationLayers) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerExt(&createInfo, nullptr, &debugMessenger)
        != VK_SUCCESS) {
      throw std::runtime_error("failed to set up debug messenger");
    }
  }
}

vk::Instance::~Instance() {
  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(debugMessenger, nullptr);
  }
  vkDestroyInstance(instance, nullptr);
}

std::vector<const char *> vk::Instance::getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  if (glfwExtensions == NULL) {
    throw std::runtime_error("failed to get required extensions");
  }

  std::vector<const char *> requiredExtensions(
      glfwExtensions, glfwExtensions + glfwExtensionCount
  );

  return requiredExtensions;
}

bool vk::Instance::checkValidationLayerSupport(
    const std::vector<const char *> &validationLayers
) {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

bool vk::Instance::checkExtensionSupport(
    const std::vector<const char *> &requiredExtensions,
    const std::vector<VkExtensionProperties> &aveilableExtensions
) {
  std::cout << "available extensions:\n";
  for (const auto &extension : aveilableExtensions) {
    std::cout << '\t' << extension.extensionName << '\n';
  }

  bool all_available = true;
  std::cout << "required extensions:\n";
  for (const char *required : requiredExtensions) {
    std::cout << std::format("\t{} ", required);

    bool is_available = false;
    for (const auto &available : aveilableExtensions) {
      if (strcmp(required, available.extensionName) == 0) {
        is_available = true;
        break;
      }
    }

    if (is_available) {
      std::cout << "AVAILABLE\n";
    } else {
      std::cout << "MISSING\n";
      all_available = false;
    }
  }

  return all_available;
}

VkResult vk::Instance::CreateDebugUtilsMessengerExt(
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger
) {
  auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")
  );

  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void vk::Instance::DestroyDebugUtilsMessengerEXT(
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator
) {
  auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")
  );

  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

void vk::Instance::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo
) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  //  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;  // Optional
}
