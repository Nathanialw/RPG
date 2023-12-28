#pragma once
#include "map"
#include "SDL2/SDL.h"
#include "spells.h"

namespace Hotbar_Structs {

  std::map<SDL_Keycode, Spells::castSpell> keybinds;

}