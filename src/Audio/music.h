#pragma once
#include "SDL2/SDL_mixer.h"
#include "string"

namespace Music {

  Mix_Music *nature = nullptr;
  Mix_Music *cave = nullptr;

  void Init() {
    //Load music
    nature = Mix_LoadMUS("assets/music/nature.ogg");
    if (nature == nullptr) {
      printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    //Load music
    cave = Mix_LoadMUS("assets/music/cave.ogg");
    if (cave == nullptr) {
      printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
  }


  void Play(std::string music) {
    if (music == "assets/music/nature.ogg")
      Mix_PlayMusic(nature, -1);
    else
      Mix_PlayMusic(cave, -1);
  }
}// namespace Music