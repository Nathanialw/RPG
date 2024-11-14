#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "array"

namespace Effects {
  enum WeaponType {
    axe,
    SIZE
  };

  std::array<std::vector<Mix_Chunk*>, SIZE> weaponSounds;

  Mix_Chunk* Load_Sound(const char* path) {
      auto sound = Mix_LoadWAV(path);
      if (sound == nullptr) {
          printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
          return nullptr; //should return a default sound that signifies an obvious error
      }
      return sound;
  }

  void Init() {
      weaponSounds[axe].emplace_back(Load_Sound("assets/sounds/Sound Library/Battle 1/Axe/Axe On Flesh/Flesh/Axe_On_Flesh_Flesh_1.wav"));
      weaponSounds[axe].emplace_back(Load_Sound("assets/sounds/Sound Library/Battle 1/Axe/Axe On Flesh/Flesh/Axe_On_Flesh_Flesh_2.wav"));
      weaponSounds[axe].emplace_back(Load_Sound("assets/sounds/Sound Library/Battle 1/Axe/Axe On Flesh/Flesh/Axe_On_Flesh_Flesh_3.wav"));
      weaponSounds[axe].emplace_back(Load_Sound("assets/sounds/Sound Library/Battle 1/Axe/Axe On Flesh/Flesh/Axe_On_Flesh_Flesh_4.wav"));
      weaponSounds[axe].emplace_back(Load_Sound("assets/sounds/Sound Library/Battle 1/Axe/Axe On Flesh/Flesh/Axe_On_Flesh_Flesh_5.wav"));
  }

  void Play(const int weaponType) {
    Mix_PlayChannel(-1, weaponSounds[weaponType][rand() % weaponSounds[weaponType].size()], 0);
  }


}// namespace Effects