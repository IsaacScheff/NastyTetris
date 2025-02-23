#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 500;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool initialize() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("SDL3 Window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return false;
    }
   
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        return false;
    }
    return true;
}

void shutdown() {
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
        renderer = nullptr;
    }
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!initialize()) {
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //draw a square
        SDL_FRect rect = {100, 100, 200, 200}; // x, y, width, height
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        SDL_RenderFillRect(renderer, &rect);

        // Update screen
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }

    shutdown();
    return 0;
}
