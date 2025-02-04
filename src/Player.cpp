#include "Player.hpp"

#include <vulkan/vulkan.h>

struct PushConstantData {
  float offsetX;
  float offsetY;
  int tileType;
  int pad;
};

Player::Player() : x(5.0f), y(5.0f) {}

void Player::handleEvent(const SDL_Event &event) {
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_UP:
      y -= 1.0f;
      break;
    case SDLK_DOWN:
      y += 1.0f;
      break;
    case SDLK_LEFT:
      x -= 1.0f;
      break;
    case SDLK_RIGHT:
      x += 1.0f;
      break;
    default:
      break;
    }
  }
}

void Player::update(float deltaTime) {}

void Player::render(VkCommandBuffer commandBuffer,
                    VkPipelineLayout pipelineLayout) {
  const float tileSize = 32.0f;
  PushConstantData push{};
  push.offsetX = x * tileSize;
  push.offsetY = y * tileSize;
  push.tileType = 4;
  push.pad = 0;
  vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
                     0, sizeof(push), &push);
  vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}
