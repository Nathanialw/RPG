#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

namespace Sounds {

    Mix_Music *music = NULL;
    Mix_Chunk *chunk = NULL;


    void init () {
        bool success = true;
        //initialize audio as well as video
        if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
        {
            printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
            printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
            success = false;
        }
        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
        //Load music
        music = Mix_LoadMUS( "21_sound_effects_and_music/beat.wav" );
        if( music == NULL )
        {
            printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
        //Load sound effects
        chunk = Mix_LoadWAV( "21_sound_effects_and_music/scratch.wav" );
        if( chunk == NULL )
        {
            printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }

    }

    void Play_Sound(){
        Mix_PlayChannel( -1, chunk, 0 );
    }


    void Play_Music(){
        Mix_PlayChannel( -1, chunk, 0 );
    }
}