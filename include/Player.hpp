#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

class Player {
public:
  float x, y;
  Player();
  void handleEvent(const SDL_Event &event);
  void update(float deltaTime);
  void render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);
};

#endif // PLAYER_HPP
