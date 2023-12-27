#pragma once
#include "map"
#include "SDL2/SDL.h"
#include "Fire/fire.h"

namespace Hotbar_Structs {

  std::map<SDL_Keycode, Fire::castSpell> keybinds;

}