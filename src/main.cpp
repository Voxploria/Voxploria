#include "Game.hpp"

int main(int argc, char* argv[]) {
    Game game;
    if (!game.init())
        return EXIT_FAILURE;

    game.run();
    game.cleanup();

    return EXIT_SUCCESS;
}
