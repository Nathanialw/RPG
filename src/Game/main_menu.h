#pragma once
#include "Game/Game_Start/character_create.h"
#include "SDL2/SDL.h"
#include "SDL_FontCache/SDL_FontCache.h"
#include "base_structs.h"
#include "camera.h"
#include "close.h"
#include "events.h"
#include "graphics.h"
#include "menu.h"
#include "pause.h"
#include "splash_screen.h"
#include "timer.h"
#include "ui_elements.h"
#include "utilities.h"
#include "video.h"

namespace Main_Menu {

  Character_Options::Customization Menu_Options() {

    Character_Options::Customization options;
    options.success = Splash_Screen::Main_Menu();
    if (!options.success) {
      return options;
    };
    
    Character_Create::Customize(options);
    return options;
  }
}// namespace Main_Menu