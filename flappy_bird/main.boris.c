#include "main.boris.h"


































int const    display_game_object(SDL_Renderer* const renderer, const SDL_RendererFlip flip, const GameObject* const game_obj)          
{
    const auto texture  = SDL_CreateTextureFromSurface(renderer, game_obj->img);

    if ( !texture )
    {
        return -1;
    }

    SDL_RenderCopyEx(renderer, texture, NULL, &game_obj->rect, 180, NULL, flip);

    SDL_DestroyTexture(texture);

    return 0;
}

int const    display_pipes(SDL_Renderer* const renderer, GameObject pipes[NUM_OF_PIPES])          
{
    for ( size_t i = 0; i < NUM_OF_PIPES; i++ )
    {
        auto flip      = SDL_FLIP_NONE;

        if ( i % 2 == 0 )
        {
            flip = SDL_FLIP_VERTICAL;
        }

        const auto err  = display_game_object(renderer, flip, &pipes[i]);

        if ( err )
        {
            return err;
        }
    }

    return 0;
}

int const    init_pipes(GameObject pipes[NUM_OF_PIPES])          
{
    auto x_add      = 0;
    for ( size_t i = 0; i < NUM_OF_PIPES; i++ )
    {
        pipes[i].img = IMG_Load("assets/pipe.png");

        if ( !pipes[i].img )
        {
            return -1;
        }

        x_add += PIPES_OFFSET;
        pipes[i].rect.w = PIPE_WIDTH;
        pipes[i].rect.x += x_add;

        //Top Pipes;
        if ( i % 2 == 0 )
        {
            const auto pipe_height  = rand() % (TOP_PIPES_UPPER_BOUND - TOP_PIPES_LOWER_BOUND + 1) + TOP_PIPES_LOWER_BOUND;
            pipes[i].rect.h = pipe_height;
            pipes[i].rect.y = 0;
        }
        //Bottom Pipes;
        else
        {
            pipes[i].rect.x = pipes[i - 1].rect.x;
            pipes[i].rect.y = PIPES_OFFSET + pipes[i - 1].rect.h;
            pipes[i].rect.h = SCREEN_HEIGHT - pipes[i].rect.y;
        }
    }

    return 0;
}

int const    deinit_pipes(GameObject pipes[NUM_OF_PIPES])          
{
    for ( size_t i = 0; i < NUM_OF_PIPES; i++ )
    {
        SDL_FreeSurface(pipes[i].img);
        pipes[i].rect = (SDL_Rect){ 0 };
    }
}

void    update_pipes(GameObject pipes[NUM_OF_PIPES])     
{
    for ( size_t i = 0; i < NUM_OF_PIPES; i++ )
    {
        pipes[i].rect.x -= PIPES_POSITION_CHANGE_PER_FRAME;
        if ( pipes[i].rect.x + PIPE_WIDTH <= 0 )
        {
            pipes[i].rect.x = (PIPES_OFFSET + PIPE_WIDTH) * 5.5;
            //Top Pipes;
            if ( i % 2 == 0 )
            {
                const auto pipe_height  = rand() % (TOP_PIPES_UPPER_BOUND - TOP_PIPES_LOWER_BOUND + 1) + TOP_PIPES_LOWER_BOUND;
                pipes[i].rect.h = pipe_height;
                pipes[i].rect.y = 0;
            }
            //Bottom Pipes;
            else
            {
                pipes[i].rect.x = pipes[i - 1].rect.x;
                pipes[i].rect.y = PIPES_OFFSET + pipes[i - 1].rect.h;
                pipes[i].rect.h = SCREEN_HEIGHT - pipes[i].rect.y;
            }
        }
    }
}

bool const    collide_with_pipes(const GameObject* const flappy, GameObject pipes[NUM_OF_PIPES])           
{
    for ( size_t i = 0; i < NUM_OF_PIPES; i++ )
    {
        for ( int x = flappy->rect.x; x <= flappy->rect.x + FLAPPY_WIDTH; x++ )
        {
            for ( int y = flappy->rect.y; y <= flappy->rect.y + FLAPPY_HEIGHT; y++ )
            {
                if ( x >= pipes[i].rect.x && x <= pipes[i].rect.x + PIPE_WIDTH && y >= pipes[i].rect.y && y <= pipes[i].rect.y + pipes[i].rect.h )
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool const    not_in_bounds(const GameObject* const flappy)           
{
    return flappy->rect.y + FLAPPY_HEIGHT <= 0 || flappy->rect.y >= SCREEN_HEIGHT;
}

int const    event_loop()          
{ 
    auto err      = 0;

    if ( SDL_Init(SDL_INIT_VIDEO) )
    {
        return -1;
    }

    auto window      = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if ( !window )
    {
        return -1;
    }

    auto renderer      = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    auto event      = (SDL_Event){ 0 };

    const auto background  = (GameObject){ .rect = { .x = 0, .y = 0, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT }, .img = IMG_Load("assets/background.png") };

    GameObject pipes[NUM_OF_PIPES] = { 0 };

    auto flappy      = (GameObject){ .rect = { .x = 0, .y = 400 - FLAPPY_HEIGHT, .w = FLAPPY_WIDTH, .h = FLAPPY_HEIGHT }, .img = IMG_Load("assets/flappy.png") };

    err = init_pipes(pipes);

    if ( !flappy.img || !background.img )
    {
        return -1;
    }

    err = display_game_object(renderer, SDL_FLIP_VERTICAL, &background);
    err = display_pipes(renderer, pipes);
    err = display_game_object(renderer, SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL, &flappy);

    if ( err )
    {
        return err;
    }

    SDL_RenderPresent(renderer);

    auto x_parabola_val      = 0.0;

    while ( true )
    {
        SDL_PollEvent(&event);

        update_pipes(pipes);

        err = display_game_object(renderer, SDL_FLIP_VERTICAL, &background);

        err = display_pipes(renderer, pipes);

        flappy.rect.y += (int)(((A_TERM * x_parabola_val * x_parabola_val) + B_TERM * x_parabola_val) / REDUCTION_TERM) - JUMP_OFFSET_TERM;

        if ( event.type == SDL_QUIT )
        {
            break;
        }
        else if ( event.type == SDL_KEYDOWN )
        {
            //handle jump;
            if ( event.key.keysym.sym == SDLK_SPACE )
            {
                x_parabola_val = 0;
            }
        }

        err = display_game_object(renderer, SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL, &flappy);

        if ( collide_with_pipes(&flappy, pipes) || not_in_bounds(&flappy) )
        {
            break;
        }

        SDL_RenderPresent(renderer);

        if ( err )
        {
            return err;
        }

        x_parabola_val++;

        SDL_Delay(DELAY);
    }

    deinit_pipes(pipes);
    SDL_FreeSurface(flappy.img);
    SDL_FreeSurface(background.img);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

int    main()    
{
    const auto err  = event_loop();

    if ( err )
    {
        fprintf(stderr, "Something went wrong: %d", err);
        return -1;
    }

    SDL_Quit();

    return 0;
}
