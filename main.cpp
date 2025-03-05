#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <map>
#include <ostream>
#include <tuple>
#include <algorithm>
#include <iostream>

constexpr int WINDOW_WIDTH = 422;
constexpr int WINDOW_HEIGHT = 750;
constexpr float OUTER_BLOCK_WIDTH = 20;
constexpr float INNER_BLOCK_WIDTH = 18;
constexpr float GRID_X = 110;
constexpr float GRID_Y = 350; 
constexpr int STEP_RATE_IN_MILLISECONDS = 500;

Uint32 lastTime = SDL_GetTicks();      
Uint32 accumulator = 0;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

enum squareColor  {
    RED,
    BLUE,
    GREEN,
    YELLOW,
    PURPLE,
    ORANGE,
    CYAN,
    BLANK
};
enum directionInput {
    LEFT,
    RIGHT
};
std::map<squareColor, std::tuple<int, int, int>> colorToRGB = {
    {RED, {255, 0, 0}},
    {GREEN, {0, 255, 0}},
    {BLUE, {0, 0, 255}},
    {YELLOW, {255, 255, 0}},
    {PURPLE, {128, 0, 128}},
    {ORANGE, {250, 156, 28}},
    {CYAN, {0, 255, 255}}
};

squareColor board_state[10][20]; //each element will be a color and this array will be used to render the squares to the screen

int piece_starting_coordinates[7][4][2] = { //seven types of pieces, four squares per piece, 2 coordinates(x and y) each
    {{4, 0},{5, 0},{4, 1},{5, 1},},
    {{4, 0},{5, 0},{6, 0},{7, 0},},
    {{4, 1},{5, 1},{5, 0},{6, 0},},
    {{4, 0},{5, 0},{5, 1},{6, 1},},
    {{4, 0},{5, 0},{6, 0},{4, 1},},
    {{4, 0},{5, 0},{6, 0},{6, 1},},
    {{4, 0},{5, 0},{6, 0},{5, 1},},
};

int active_coordinates[4][2]; //the four places in the grid that the current active piece occupy
squareColor active_color;

bool is_not_active_coord(int x, int y) {
    for(int i = 0; i < 4; i++) {
        if(active_coordinates[i][0] == x && active_coordinates[i][1] == y) {
            return false;
        }
    }
    return true;
}

bool can_active_move_down() { // also need to account for already being on bottom row
    for(int i = 0; i < 4; i++) {
        int current_X = active_coordinates[i][0];
        int new_Y = active_coordinates[i][1] + 1;
        if((board_state[current_X][new_Y] != BLANK && is_not_active_coord(current_X, new_Y)) || new_Y >= 20){
            return false;
        }
    }
    return true;
}

void move_active_down() {
    for(int i = 0; i < 4; i++) {
        board_state[active_coordinates[i][0]][active_coordinates[i][1]] = BLANK;
        active_coordinates[i][1]++;
    }
    for(int i = 0; i < 4; i++) {
        board_state[active_coordinates[i][0]][active_coordinates[i][1]] = active_color;
    }
}

void draw_tetris_square(SDL_Renderer* renderer, float x_cor, float y_cor, squareColor sColor) { //TODO: arg for either place in the grid or coordinates on screen to render square
    int rgbFirst = std::get<0>(colorToRGB[sColor]);
    int rgbSecond = std::get<1>(colorToRGB[sColor]);
    int rgbThird = std::get<2>(colorToRGB[sColor]);
    SDL_FRect outline = {x_cor, y_cor, OUTER_BLOCK_WIDTH, OUTER_BLOCK_WIDTH}; // x, y, width, height
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white color
    SDL_RenderFillRect(renderer, &outline);

    SDL_FRect rect = {x_cor + 1, y_cor + 1, INNER_BLOCK_WIDTH, INNER_BLOCK_WIDTH}; // x, y, width, height
    SDL_SetRenderDrawColor(renderer, rgbFirst, rgbSecond, rgbThird, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_tetronimo(SDL_Renderer* renderer, int coords[4][2], squareColor color) {//wait, will this actually be used if we're just rendering squares based on boad_state??? 
    for(int i = 0; i < 4; i++) {
        draw_tetris_square(renderer, GRID_X + (coords[i][0] * OUTER_BLOCK_WIDTH), GRID_Y + (coords[i][1] * OUTER_BLOCK_WIDTH), color);
    }
}

void draw_tetris_grid(SDL_Renderer* renderer) { 
    for(int i = 0; i < 11; i++) { //vertical lines
        SDL_RenderLine(renderer, GRID_X + (OUTER_BLOCK_WIDTH * i), GRID_Y, GRID_X + (OUTER_BLOCK_WIDTH * i), WINDOW_HEIGHT);
    }
    for(int i = 0; i < 21; i++) { //horizontal lines
        SDL_RenderLine(renderer, GRID_X, GRID_Y + (OUTER_BLOCK_WIDTH * i), 310, GRID_Y + (OUTER_BLOCK_WIDTH * i)); //310 is grid_x + (OUTER_BLOCK_WIDTH * number of rows(20))
    }
}

void draw_from_board_state(SDL_Renderer* renderer) {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 20; j++) {
            if(board_state[i][j] != BLANK) {
                draw_tetris_square(renderer, GRID_X + (i * OUTER_BLOCK_WIDTH), GRID_Y + (j * OUTER_BLOCK_WIDTH), board_state[i][j]);
            }
        }
    }
}

void spawn_new_piece() {
    //need to randomly pick 1 - 7 (excluding previous piece)
    //then draw piece and set active squares
}

bool can_active_move_horizontal(directionInput direction) {
    int direction_adjustment = (direction == RIGHT ? 1 : 0); //moving right adds to Y value, left subtracts from it
    for(int i = 0; i < 4; i++) {
        int current_Y = active_coordinates[i][1];
        int new_X = active_coordinates[i][0] + direction_adjustment;
        if((board_state[new_X][current_Y] != BLANK && is_not_active_coord(new_X, current_Y)) || new_X >= 11 || new_X <= -1){
            return false;
        }
    }
    return true;
}

void slide_active_piece(directionInput direction) {
}

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

    for (int i = 0; i < 10; i++) {
        std::fill_n(board_state[i], 20, BLANK);
    }

    active_coordinates[0][0] =  4;       
    active_coordinates[0][1] =  0;       
    active_coordinates[1][0] =  5;       
    active_coordinates[1][1] =  0;       
    active_coordinates[2][0] =  5;       
    active_coordinates[2][1] =  1;       
    active_coordinates[3][0] =  6;       
    active_coordinates[3][1] =  1;       
    board_state[4][17] = BLUE;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false; // Exit the loop if the window is closed
                    break;

                case SDL_EVENT_KEY_DOWN:
                    // Check which arrow key was pressed
                    switch (event.key.scancode) {
                        case SDL_SCANCODE_RIGHT:
                            std::cout << "Right arrow key pressed!" << std::endl;
                            if(can_active_move_horizontal(RIGHT)) {
                                std::cout << "YES!" << std::endl;
                            }else {
                                std::cout << "No!" << std::endl;
                            }
                            break;
                        case SDL_SCANCODE_UP:
                            std::cout << "Up arrow key pressed!" << std::endl;
                            break;
                        case SDL_SCANCODE_LEFT:
                            std::cout << "Left arrow key pressed!" << std::endl;
                            break;
                        case SDL_SCANCODE_DOWN:
                            std::cout << "Down arrow key pressed!" << std::endl;
                            break;
                        default:
                            // Ignore other keys
                            break;
                    }
                    break;

                default:
                    // Ignore other events
                    break;
            }
        }

        Uint32 currentTime = SDL_GetTicks(); // Current time in milliseconds
        Uint32 deltaTime = currentTime - lastTime; // Time since last frame
        lastTime = currentTime; // Update lastTime for the next frame
        accumulator += deltaTime;
        
        if(accumulator >= STEP_RATE_IN_MILLISECONDS) {
            if(can_active_move_down()) {
                move_active_down();
            }
            accumulator -= STEP_RATE_IN_MILLISECONDS; 
        } 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer); 


        draw_from_board_state(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        draw_tetris_grid(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }

    shutdown();
    return 0;
}
