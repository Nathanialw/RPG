#pragma once
#include "SDL2/SDL_mixer.h"
#include "effects.h"
#include "music.h"

namespace Sound {

  void Init() {
    bool success = true;
    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
      printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
      success = false;
    }
    Music::Init();
    Effects::Init();
  }

  void Update_Volume() {
    if (Debug::settings[Debug::Settings::volume]) {
      Mix_Volume(-1, 64);
      Mix_VolumeMusic(64);
      //      Mix_VolumeChunk(Mix_Chunk *chunk, int volume);
    } else {
      Mix_Volume(-1, 0);
      Mix_VolumeMusic(0);
    }
  }
}// namespace Sound