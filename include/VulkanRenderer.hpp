#ifndef VULKAN_RENDERER_HPP
#define VULKAN_RENDERER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <stdexcept>
#include <string>
#include <vector>

struct PushConstantData {
    float offsetX;
    float offsetY;
    int   tileType;
    int   pad;
};

class VulkanRenderer {
  public:
    VulkanRenderer()  = default;
    ~VulkanRenderer() = default;

    void                  init(SDL_Window* window);
    void                  cleanup();
    void                  drawFrame();
    void                  recreateSwapChain();

    VkDevice              device;

    VkBuffer              quadVertexBuffer;
    VkDeviceMemory        quadVertexBufferMemory;

    SDL_Window*           window;
    VkPipelineLayout      pipelineLayout;
    VkPipeline            graphicsPipeline;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool      descriptorPool;
    VkDescriptorSet       descriptorSet;

    // Texture resources.
    VkImage        textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView    textureImageView;
    VkSampler      textureSampler;

    // Helper buffer/image functions.
    void            createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void            createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                                VkDeviceMemory& imageMemory);
    VkImageView     createImageView(VkImage image, VkFormat format);
    void            transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void            copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void            copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    VkCommandBuffer beginSingleTimeCommands();
    void            endSingleTimeCommands(VkCommandBuffer commandBuffer);

  private:
    uint32_t                     findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkInstance                   instance;
    VkSurfaceKHR                 surface;
    VkPhysicalDevice             physicalDevice = VK_NULL_HANDLE;
    VkQueue                      graphicsQueue;
    VkQueue                      presentQueue;
    VkSwapchainKHR               swapChain;
    std::vector<VkImage>         swapChainImages;
    VkFormat                     swapChainImageFormat;
    VkExtent2D                   swapChainExtent;
    std::vector<VkImageView>     swapChainImageViews;
    VkRenderPass                 renderPass;
    std::vector<VkFramebuffer>   swapChainFramebuffers;
    VkCommandPool                commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore>     imageAvailableSemaphores;
    std::vector<VkSemaphore>     renderFinishedSemaphores;
    std::vector<VkFence>         inFlightFences;
    size_t                       currentFrame         = 0;
    const int                    MAX_FRAMES_IN_FLIGHT = 2;

    uint32_t                     graphicsQueueFamilyIndex = 0;

    // Helper functions.
    void              createInstance(SDL_Window* window);
    void              createSurface(SDL_Window* window);
    void              pickPhysicalDevice();
    void              createLogicalDevice();
    void              createSwapChain(SDL_Window* window);
    void              createImageViews();
    void              createRenderPass();
    void              createDescriptorSetLayout();
    void              createGraphicsPipeline();
    void              createFramebuffers();
    void              createCommandPool();
    void              createCommandBuffers();
    void              recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void              createSyncObjects();
    void              createQuadVertexBuffer();

    void              createTextureImage();
    void              createTextureImageView();
    void              createTextureSampler();
    void              createDescriptorPool();
    void              createDescriptorSet();

    std::vector<char> readFile(const std::string& filename);
    VkShaderModule    createShaderModule(const std::vector<char>& code);
};

#endif // VULKAN_RENDERER_HPP
