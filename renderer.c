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

int Get_Direction_Input();

void Get_Ball_Direction(uint32_t *pixels, int *ball_position_offset) {
    
    int x_offset = 0;
    int y_offset = 0;

    return;
}

int main() {

// ----------------------
// --- SDL Initialization
// ----------------------

    // Initalizations
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("PONG", 1920, 1080, SDL_WINDOW_RESIZABLE); 
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 1920, 1080);
    uint32_t *pixels;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Loop variables
    int pitch;
    const int PADDLE_HEIGHT = 80;
    const int PADDLE_WIDTH = 20;
    SDL_Event event;
    int isQuitTrue = 0;
    int paddle_center_offset = (SCREEN_HEIGHT / 2); // Starting offset (center at half of screen height)
    int ball_position_offset[2] = {0, 0};
    int ball_x_direction = -1;

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
        paddle_center_offset += Get_Direction_Input(); // Get Input direction to move paddle, if any
                                                    
        // Paddle bounding
        if (paddle_center_offset - (PADDLE_HEIGHT / 2) < 5) {
            paddle_center_offset = PADDLE_HEIGHT / 2 + 5; // move to 5px away from top
        }

        if (paddle_center_offset > (SCREEN_HEIGHT - (PADDLE_HEIGHT / 2)) - 5) {
            paddle_center_offset = SCREEN_HEIGHT - (PADDLE_HEIGHT / 2) - 5;
        }

        // Update new paddle position
        for (int row = paddle_center_offset - (PADDLE_HEIGHT / 2); row < paddle_center_offset + (PADDLE_HEIGHT / 2); row++) {
            for (int col = 20; col < 20 + PADDLE_WIDTH; col++) {
                pixels[col + (row * (pitch / 4))] = 0xFFFFFFFF;
            }
        }

        // --- BALL LOGIC
        for (int row = SCREEN_HEIGHT / 2 - 5; row < SCREEN_HEIGHT / 2 + 5; row++) {
            for (int col = SCREEN_WIDTH / 2 - 5; col < SCREEN_WIDTH / 2 + 5; col++) {
                
                //uint32_t ball_pos_value = pixels[col + (row * (pitch / 4)) + ball_position_offset[0]];
                //printf("The formatted hexadecimal value is: 0x%08" PRIx32 "\n", ball_pos_value);

                pixels[col + (row * (pitch / 4)) + ball_position_offset[0]] = 0xFFFFFFFF;
                

                //if (ball_position_offset[0] >= ((SCREEN_WIDTH / 2) - 11)) {
                //    ball_x_direction = -1;
                //}

                //if (ball_position_offset[0] <=  -1 * ((SCREEN_WIDTH / 2))) {
                //    ball_x_direction = 1;
                //}
                
            }
        }

        ball_position_offset[0] += ball_x_direction;

        SDL_UnlockTexture(texture);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    }

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



