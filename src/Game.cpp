#include "Game.hpp"

#include <chrono>
#include <iostream>

// Global pointers (used in VulkanRenderer::recordCommandBuffer for simplicity)
World*  gWorld  = nullptr;
Player* gPlayer = nullptr;

Game::Game() : window(nullptr), running(true) {}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("Voxploria - InDev", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        return false;
    }

    try {
        renderer.init(window);
    } catch (const std::runtime_error& e) {
        std::cerr << "Vulkan initialization error: " << e.what() << "\n";
        return false;
    }

    // Set global pointers for use in the renderer.
    gWorld  = &world;
    gPlayer = &player;

    return true;
}

void Game::run() {
    auto      lastTime       = std::chrono::high_resolution_clock::now();
    float     printTimer     = 0.0f; // For logging player position.
    Uint32    lastResizeTime = SDL_GetTicks();
    bool      pendingResize  = false;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    pendingResize  = true;
                    lastResizeTime = SDL_GetTicks();
                }
            }
            gPlayer->handleEvent(event);
        }

        if (pendingResize && (SDL_GetTicks() - lastResizeTime > 200)) {
            std::cout << "Resizing: recreating swap chain..." << std::endl;
            renderer.recreateSwapChain();
            pendingResize = false;
        }

        auto  currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime   = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime          = currentTime;

        gPlayer->update(deltaTime);
        gWorld->update(deltaTime);

        try {
            renderer.drawFrame();
        } catch (const std::runtime_error& e) {
            std::cerr << "Draw frame error: " << e.what() << "\n";
            running = false;
        }

        printTimer += deltaTime;
        if (printTimer >= 1.0f) {
            std::cout << "Player position: (" << gPlayer->x << ", " << gPlayer->y << ")\n";
            printTimer = 0.0f;
        }
    }

    vkDeviceWaitIdle(renderer.device);
}

void Game::cleanup() {
    renderer.cleanup();
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}
