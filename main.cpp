#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr int WINDOW_WIDTH = 750;
constexpr int WINDOW_HEIGHT = 750;
constexpr float OUTER_BLOCK_WIDTH = 40;
constexpr float INNER_BLOCK_WIDTH = 32;

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

void draw_tetris_square(SDL_Renderer* renderer, float x, float y) { //TODO: arg for either place in the grid or coordinates on screen to render square
    
    SDL_FRect outline = {x, y, OUTER_BLOCK_WIDTH, OUTER_BLOCK_WIDTH}; // x, y, width, height
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white color
    SDL_RenderFillRect(renderer, &outline);

    SDL_FRect rect = {x + 4, y + 4, INNER_BLOCK_WIDTH, INNER_BLOCK_WIDTH}; // x, y, width, height
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_RenderFillRect(renderer, &rect);
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
        
        draw_tetris_square(renderer, 100, 100);        /*//draw a square*/
        draw_tetris_square(renderer, 200, 200);        /*//draw a square*/
        draw_tetris_square(renderer, 200, 100);        /*//draw a square*/
        draw_tetris_square(renderer, 100, 200);        /*//draw a square*/

        // Update screen
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }

    shutdown();
    return 0;
}
