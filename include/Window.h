#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <string>
#include <stdexcept>

class Window {
public:
    Window(const std::string& title);
    ~Window();

    bool IsRunning();
    void ProcessEvents();

private:
    SDL_Window* window;
    SDL_Event event;
    bool running;
};

#endif
