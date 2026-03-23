#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

struct ball_position {
    int x;
    int y; 
};

struct paddle {
    int center_position;
};

// PADDLE Variables
const int PADDLE_HEIGHT = 80;
const int PADDLE_WIDTH = 20;
const int PADDLE_WALL_OFFSET = 20;

int Get_Direction_Input();
int Get_AI_Input(struct ball_position ball_pos, struct paddle *paddle_ai);

int main() {

// ----------------------
// --- SDL Initialization
// ----------------------

    // SDL Initalizations
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("PONG", 1920, 1080, SDL_WINDOW_RESIZABLE); 
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 1920, 1080);
    uint32_t *pixels;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    int pitch;

    // Loop variables
    SDL_Event event;
    int isQuitTrue = 0;

    // Ball initializations
    struct ball_position ball_pos = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    int ball_x_direction = -1;

    // Paddle initializations
    struct paddle paddle_player = {SCREEN_HEIGHT / 2};
    struct paddle paddle_ai = {SCREEN_HEIGHT / 2};

    // Main loop
    while (!isQuitTrue) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isQuitTrue = 1;
                printf("QUIT DETECTED\n");
                break;
            }
        }

        // Get the pixel data to modify
        SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch);
    
        SDL_memset4(pixels, 0xFF000000, ((pitch / 4) * SCREEN_HEIGHT)); // Set screen to black

        // --- PADDLE LOGIC
        paddle_player.center_position += Get_Direction_Input(); // Get Input direction to move paddle, if any
                                                    
        // Paddle bounding
        if (paddle_player.center_position - (PADDLE_HEIGHT / 2) < 5) {
            paddle_player.center_position = PADDLE_HEIGHT / 2 + 5; // move to 5px away from top
        }

        if (paddle_player.center_position > (SCREEN_HEIGHT - (PADDLE_HEIGHT / 2)) - 5) {
            paddle_player.center_position = SCREEN_HEIGHT - (PADDLE_HEIGHT / 2) - 5;
        }

        // Update new paddle positions
        // PLAYER 1
        for (int row = paddle_player.center_position - (PADDLE_HEIGHT / 2); row < paddle_player.center_position + (PADDLE_HEIGHT / 2); row++) {
            for (int col = PADDLE_WALL_OFFSET; col < PADDLE_WALL_OFFSET + PADDLE_WIDTH; col++) {
                pixels[col + (row * (pitch / 4))] = 0xFFFFFFFF;
            }
        }

        // AI
        for (int row = paddle_ai.center_position - (PADDLE_HEIGHT / 2); row < paddle_ai.center_position + (PADDLE_HEIGHT / 2); row++) {
            for (int col = SCREEN_WIDTH - PADDLE_WALL_OFFSET - PADDLE_WIDTH; col < SCREEN_WIDTH - PADDLE_WALL_OFFSET; col++) {
                pixels[col + (row * (pitch / 4))] = 0xFFFFFFFF;
            }
        }

        // --- BALL LOGIC
        for (int row = ball_pos.y - 7; row < ball_pos.y + 7; row++) {
            for (int col = ball_pos.x - 7; col < ball_pos.x + 7; col++) {
                
                pixels[col + (row * (pitch / 4))] = 0xFFFFFFFF;
                
            }
        }
 
        // Ball bounce off player paddle logic
        if (ball_pos.x <= PADDLE_WALL_OFFSET + PADDLE_WIDTH && (ball_pos.y > paddle_player.center_position - PADDLE_HEIGHT && ball_pos.y < paddle_player.center_position + PADDLE_HEIGHT)) {
            ball_x_direction = 1;
        }

        // Ball bounce off ai paddle
        if (ball_pos.x >= SCREEN_WIDTH - PADDLE_WALL_OFFSET - PADDLE_WIDTH && (ball_pos.y > (paddle_ai.center_position - PADDLE_HEIGHT && ball_pos.y < paddle_ai.center_position + PADDLE_HEIGHT))) {
            ball_x_direction = -2;
        }

        if (ball_pos.x <= 0 || ball_pos.x >= SCREEN_WIDTH) {

        }

        // Move the balls x position based on direction variable
        ball_pos.x += ball_x_direction;

        // Unlock the texture and render it
        SDL_UnlockTexture(texture);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    }

    // Destroy SDL stuff and free memory
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
    return 0;
}

int Get_Direction_Input() {

    const bool *key_states = SDL_GetKeyboardState(NULL);
    int direction = 0;

    if (key_states[SDL_SCANCODE_W]) {
        direction += -1;
    }

    if (key_states[SDL_SCANCODE_S]) {
        direction += 1;
    }

    return direction;
}

int Get_AI_Input(struct ball_position ball_pos, struct paddle *paddle_ai) {
    
    int direction = 0;

    if (ball_pos.y < (paddle_ai->center_position - PADDLE_HEIGHT)) {

    }

    return direction;
}


