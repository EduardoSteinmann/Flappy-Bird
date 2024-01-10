#ifndef MAINBORIS_H
#define MAINBORIS_H
#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define B_TERM 200
#define A_TERM 1
#define REDUCTION_TERM 180
#define JUMP_OFFSET_TERM 10
#define TOP_PIPES_UPPER_BOUND 500
#define TOP_PIPES_LOWER_BOUND 250
#define DELAY 5
#define PIPE_WIDTH 50
#define PIPES_OFFSET 150
#define PIPES_POSITION_CHANGE_PER_FRAME 1
#define NUM_OF_PIPES 8
#define FLAPPY_WIDTH 49
#define FLAPPY_HEIGHT 27
#undef main
typedef struct GameObject GameObject;
struct GameObject
{
    SDL_Rect rect;
    SDL_Surface* img;
};
int const    display_game_object(SDL_Renderer* const renderer, const SDL_RendererFlip flip, const GameObject* const game_obj)          ;
int const    display_pipes(SDL_Renderer* const renderer, GameObject pipes[NUM_OF_PIPES])          ;
int const    init_pipes(GameObject pipes[NUM_OF_PIPES])          ;
int const    deinit_pipes(GameObject pipes[NUM_OF_PIPES])          ;
void    update_pipes(GameObject pipes[NUM_OF_PIPES])     ;
bool const    collide_with_pipes(const GameObject* const flappy, GameObject pipes[NUM_OF_PIPES])           ;
bool const    not_in_bounds(const GameObject* const flappy)           ;
int const    event_loop()          ;
int    main()    ;
#endif
