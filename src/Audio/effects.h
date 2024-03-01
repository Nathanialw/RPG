#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

namespace Effects {

  struct Sounds {
    Mix_Chunk *chunk;
  };

  std::vector<Sounds> chunks;

  void Init() {
    //Load sound effects
    Sounds sound{};
    sound.chunk = Mix_LoadWAV("assets/sounds/Sound Library/_Combined Clips (Battle)/Axe/OnFlesh/AxeOnFlesh_0.wav");
    chunks.emplace_back(sound);
    sound.chunk = Mix_LoadWAV("assets/sounds/Sound Library/_Combined Clips (Battle)/Axe/OnFlesh/AxeOnFlesh_1.wav");
    chunks.emplace_back(sound);
    sound.chunk = Mix_LoadWAV("assets/sounds/Sound Library/_Combined Clips (Battle)/Axe/OnFlesh/AxeOnFlesh_2.wav");
    chunks.emplace_back(sound);
    sound.chunk = Mix_LoadWAV("assets/sounds/Sound Library/_Combined Clips (Battle)/Axe/OnFlesh/AxeOnFlesh_3.wav");
    chunks.emplace_back(sound);
    sound.chunk = Mix_LoadWAV("assets/sounds/Sound Library/_Combined Clips (Battle)/Axe/OnFlesh/AxeOnFlesh_4.wav");
    chunks.emplace_back(sound);

    if (sound.chunk == nullptr) {
      printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
  }

  void Play() {
    Mix_PlayChannel(-1, chunks[rand() % chunks.size()].chunk, 0);
  }


}// namespace Effects