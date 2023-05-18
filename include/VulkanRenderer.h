#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

class VulkanRenderer {
public:
    VulkanRenderer();
    ~VulkanRenderer();

    void Initialize();
    void Cleanup();

private:
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;

    void CreateInstance();
    void SelectPhysicalDevice();
    void CreateLogicalDevice();
};

#endif
