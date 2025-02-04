#include "World.hpp"

#include <vulkan/vulkan.h>

#include <iostream>

struct PushConstantData {
  float offsetX;
  float offsetY;
  int tileType;
  int pad;
};

World::World() {
  tiles.resize(HEIGHT, std::vector<int>(WIDTH, 0));
  // Bottom row: grass (tile 1)
  // Next row: dirt (tile 2)
  // Third row from bottom: stone (tile 3)
  for (int i = 0; i < WIDTH; ++i) {
    tiles[HEIGHT - 1][i] = 1;
    tiles[HEIGHT - 2][i] = 2;
    tiles[HEIGHT - 3][i] = 3;
  }
}

void World::update(float deltaTime) {}

void World::render(VkCommandBuffer commandBuffer,
                   VkPipelineLayout pipelineLayout) {
  const float tileSize = 32.0f;
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      int tile = tiles[y][x];
      if (tile != 0) {
        PushConstantData push{};
        push.offsetX = x * tileSize;
        push.offsetY = y * tileSize;
        push.tileType = tile; // 1=grass, 2=dirt, 3=stone
        push.pad = 0;
        vkCmdPushConstants(commandBuffer, pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(push), &push);
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
      }
    }
  }
}
