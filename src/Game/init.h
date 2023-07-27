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

  void Init_World () {
    Clear_Events();
      Init_Tiles_Array();
    Collision::init_Collison();
    Init_Zone(World::zone);
    Maps::Create_Map();
    Load_Object_List::Load_Entities();
    //World::Generate_Map();
    Character_Stats::Init_UI(World::zone);
    Dynamic_Quad_Tree::Fill_Quad_Tree(World::zone);
    UI_Spellbook::Init_UI();
    Action_Bar::Create_Action_Bar(World::zone);
    Menu::Init();
    SQLite_Dialogue::Init_Dialogue();
    Video::Run_Audio("assets/music/nature.ogg");
 
  }

  void Init_Client() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      //SDL_Log(SDL_GetError());
    }
    Mix_Init(0);
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);

    //	Joystick::JoystickInit();
    TTF_Init();

    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_ShowCursor(SDL_DISABLE );

    Graphics::running = true;

    Graphics::createGraphicsContext(World::zone);

    Main_Menu::Menu_Options();

    Init_World();
  }
}
