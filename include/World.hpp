#ifndef WORLD_HPP
#define WORLD_HPP

#include <vulkan/vulkan.h>

#include <vector>

class World {
public:
  static const int WIDTH = 50;
  static const int HEIGHT = 30;

  World();
  void update(float deltaTime);
  void render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);

  // 0 = empty, 1 = grass, 2 = dirt, 3 = stone.
  std::vector<std::vector<int>> tiles;
};

#endif // WORLD_HPP
