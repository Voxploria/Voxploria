#include "VulkanRenderer.h"
#include <algorithm>

VulkanRenderer::VulkanRenderer() : instance(VK_NULL_HANDLE), physicalDevice(VK_NULL_HANDLE), device(VK_NULL_HANDLE) {}

VulkanRenderer::~VulkanRenderer() {
    Cleanup();
}

void VulkanRenderer::Initialize() {
    try {
        CreateInstance();
        SelectPhysicalDevice();
        CreateLogicalDevice();
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        Cleanup();
        throw;
    }
}

void VulkanRenderer::Cleanup() {
    if (device != VK_NULL_HANDLE) {
        vkDestroyDevice(device, nullptr);
        device = VK_NULL_HANDLE;
    }

    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE;
    }
}

void VulkanRenderer::CreateInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Voxploria";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "None";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance.");
    }
}

void VulkanRenderer::SelectPhysicalDevice() {
    uint32_t deviceCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (result != VK_SUCCESS || deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    result = vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to enumerate physical devices.");
    }

    VkPhysicalDevice preferredDevice = VK_NULL_HANDLE;
    VkPhysicalDevice fallbackDevice = VK_NULL_HANDLE;

    for (const auto& device : physicalDevices) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        std::cout << "Physical Device: " << deviceProperties.deviceName << std::endl;

        if (deviceProperties.apiVersion >= VK_API_VERSION_1_0) {
            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                preferredDevice = device;
                break;
            }

            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
                fallbackDevice = device;
            }
        }
    }

    if (preferredDevice != VK_NULL_HANDLE) {
        physicalDevice = preferredDevice;
    }
    else if (fallbackDevice != VK_NULL_HANDLE) {
        physicalDevice = fallbackDevice;
    }
    else {
        throw std::runtime_error("Failed to find a suitable GPU.");
    }
}

void VulkanRenderer::CreateLogicalDevice() {
    std::vector<VkQueueFamilyProperties> queueFamilies;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    queueFamilies.resize(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    std::vector<int> queueFamilyIndices;
    const float queuePriority = 1.0f;

    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndices.push_back(i);
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            queueFamilyIndices.push_back(i);
        }

        if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            queueFamilyIndices.push_back(i);
        }
    }

    std::sort(queueFamilyIndices.begin(), queueFamilyIndices.end());
    queueFamilyIndices.erase(std::unique(queueFamilyIndices.begin(), queueFamilyIndices.end()), queueFamilyIndices.end());

    for (int queueFamilyIndex : queueFamilyIndices) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    std::vector<const char*> deviceExtensions = {};

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device.");
    }

    device = logicalDevice;
}
