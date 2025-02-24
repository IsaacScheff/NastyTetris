#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <map>
#include <tuple>

constexpr int WINDOW_WIDTH = 750;
constexpr int WINDOW_HEIGHT = 750;
constexpr float OUTER_BLOCK_WIDTH = 40;
constexpr float INNER_BLOCK_WIDTH = 32;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

enum squareColor  {
    RED,
    BLUE,
    GREEN,
    YELLOW,
    PURPLE,
    ORANGE
};

std::map<squareColor, std::tuple<int, int, int>> colorToRGB = {
    {RED, {255, 0, 0}},
    {GREEN, {0, 255, 0}},
    {BLUE, {0, 0, 255}},
    {YELLOW, {255, 255, 0}},
    {PURPLE, {128, 0, 128}},
    {ORANGE, {250, 156, 28}}
};

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

void draw_tetris_square(SDL_Renderer* renderer, float x_cor, float y_cor, squareColor sColor) { //TODO: arg for either place in the grid or coordinates on screen to render square
    int rgbFirst = std::get<0>(colorToRGB[sColor]);
    int rgbSecond = std::get<1>(colorToRGB[sColor]);
    int rgbThird = std::get<2>(colorToRGB[sColor]);
    SDL_FRect outline = {x_cor, y_cor, OUTER_BLOCK_WIDTH, OUTER_BLOCK_WIDTH}; // x, y, width, height
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white color
    SDL_RenderFillRect(renderer, &outline);

    SDL_FRect rect = {x_cor + 4, y_cor + 4, INNER_BLOCK_WIDTH, INNER_BLOCK_WIDTH}; // x, y, width, height
    SDL_SetRenderDrawColor(renderer, rgbFirst, rgbSecond, rgbThird, 255);
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
        
        draw_tetris_square(renderer, 100, 100, RED);
        draw_tetris_square(renderer, 200, 200, BLUE);       
        draw_tetris_square(renderer, 200, 100, GREEN);        
        draw_tetris_square(renderer, 100, 200, PURPLE);        

        // Update screen
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }

    shutdown();
    return 0;
}
