#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include "Joystick.h"
#include <SDL2/SDL.h>
#include "graphics.h"
#include "rendering.h"
#include "event_handler.h"
#include "collision.h"
#include "debug_system.h"
#include "ai_control.h"
#include "interface.h"
#include "movement.h"
#include "spells.h"
#include "unit_positions.h"
#include "unit_status.h"
#include "formation_collisions.h"
#include "squad_control.h"
#include "character_stats.h"
#include "map.h"
#include "texture_packer.h"
#include "main_menu.h"
#include "menu.h"
#include "world.h"
#include "load_object_list.h"
#include "interface.h"
#include "ui_frames.h"
#include "dynamic_entity_loader.h"

namespace Init {

  std::string batch = "1";

  void Clear_Events() {
    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
  }

  void Init_World(Character_Stats::Customization &options) {
    Clear_Events();
    Debug::Load_Settings();
    Init_Tiles_Array();
    UI_Frames::Load_Buildings();
    Collision::init_Collison();
    Init_Zone(World::zone);
    World::Generate_Region();
    Load_Object_List::Load_Entities();
    World::Init_Tile_Objects();
    //World::Generate_Map();
    Character_Stats::Init_UI(World::zone);
    Character_Stats::Init_Player(World::zone, options);
    Dynamic_Quad_Tree::Fill_Quad_Tree(World::zone);
    UI_Spellbook::Init_UI();
    Action_Bar::Create_Action_Bar(World::zone);
    Menu::Init();
    Menu_Options::Load_Options();
    SQLite_Dialogue::Init_Dialogue();
    Video::Run_Audio("assets/music/nature.ogg");
  }

  bool Init_Client() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      //SDL_Log(SDL_GetError());
    }
    Mix_Init(0);
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);

    //	Joystick::JoystickInit();
    TTF_Init();

    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_ShowCursor(SDL_DISABLE);

    Graphics::running = true;

    Graphics::createGraphicsContext(World::zone);

    Character_Stats::Customization options = Main_Menu::Menu_Options();
    if (!options.success) {
      return false;
    }

    Init_World(options);
    return true;
  }
}
