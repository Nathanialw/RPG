#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include "Joystick.h"
#include "ai_control.h"
#include "character_stats.h"
#include "collision.h"
#include "debug_system.h"
#include "dynamic_entity_loader.h"
#include "event_handler.h"
#include "formation_collisions.h"
#include "graphics.h"
#include "interface.h"
#include "load_object_list.h"
#include "main_menu.h"
#include "map.h"
#include "menu.h"
#include "movement.h"
#include "rendering.h"
#include "spells.h"
#include "squad_control.h"
#include "texture_packer.h"
#include "ui_frames.h"
#include "unit_positions.h"
#include "unit_status.h"
#include "world.h"
#include <SDL2/SDL.h>

namespace Init {

  std::string batch = "1";
  struct Game {
    bool success = false;
    Menu::Menu menu;
  };


  void Clear_Events() {
    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
  }

  Character_Options::Customization Character_Create() {
    return Main_Menu::Menu_Options();
  }

  void Init_Data() {
    Scene::Init_Zone();
    Init_Tiles_Array();
    Debug::Load_Settings();
    UI_Frames::Load_Buildings();
    //this one should be different for whichever class you create
    World::Generate_Region();
    Load_Object_List::Load_Entities();
    Collision::init_Collison();
    Menu::Init();
    Menu_Options::Load_Options();
    SQLite_Dialogue::Init_Dialogue();
    Character_Stats::Init_Items(World::zone);
  }

  void Create_Game_entities(Character_Options::Customization &options) {
    //for when we clear the registry
    Mouse::Init_mouse(World::zone);
    UI_Spellbook::Init_UI(World::zone);
    Character_Stats::Init_UI(World::zone);
    Character_Stats::Init_Player(World::zone, options);
    World::Init_Tile_Objects();
    Dynamic_Quad_Tree::Fill_Quad_Tree(World::zone);
    Action_Bar::Create_Action_Bar(World::zone);
  };

  void Restart_Game() {
    Collision::close_collision();
    World::close_zone();
    Dynamic_Quad_Tree::treeObjects.clear();
  }

  Game Init_World(Game &game) {
    Clear_Events();
    Video::Run_Audio("assets/music/nature.ogg");
    return game;
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
    SDL_ShowCursor(SDL_DISABLE);

    Graphics::running = true;
    Graphics::createGraphicsContext(World::zone);
    Init_Data();
  }

  Game Start_Game() {
    Game game;
    SDL_RenderSetScale(Graphics::renderer, 1.0f, 1.0f);
    Character_Options::Customization options = Character_Create();
    if (!options.success) {
      return game;
    }
    Create_Game_entities(options);
    Init_World(game);
    return game;
  }
}// namespace Init
