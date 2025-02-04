#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>

#include "Player.hpp"
#include "VulkanRenderer.hpp"
#include "World.hpp"

class Game {
  public:
    Game();
    bool   init();
    void   run();
    void   cleanup();

    World  world;
    Player player;

  private:
    SDL_Window*    window;
    bool           running;
    VulkanRenderer renderer;
};

#endif // GAME_HPP
