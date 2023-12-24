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
    bool overworld = false;
    bool cave = false;
  };

  struct Loaded {
    bool overworld = false;
    bool cave = false;
  };

  void Clear_Events() {
    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
  }

  Character_Options::Customization Character_Create() {
    return Main_Menu::Menu_Options();
  }

  Character_Options::Customization options;

  void Recreate_Player(entt::registry &zone, World::GameState &state) {
    Character_Stats::Recreate_Player(zone, state, options);
  }

  void Init_Data() {
    Graphics::Load_Textures();

    Scene::Init_Zone();
    Init_Tiles_Array();
    Debug::Load_Settings();
    UI_Frames::Load_Buildings();
    //this one should be different for whichever class you create
    Maps::Generate_Region();
    Load_Object_List::Load_Entities();
    Menu::Init();
    Menu_Options::Load_Options();
    SQLite_Dialogue::Init_Dialogue();
    Character_Stats::Init_Items();
  }

  //only needs to be fired once per zone
  void Create_Game_entities(entt::registry &zone, World::GameState &state, Character_Options::Customization &options) {
    //for when we clear the registry
    Collision::init_Collison(state);
    Item_Component::emptyEquipSlot[state] = Graphics::Create_Icon_Entity(zone, Graphics::emptyBagIcon, Graphics::bagSlotBorder, Component::Icon_Type::item);
    Graphics::defaultIcon = Graphics::Create_Icon_Entity(zone, Graphics::default_icon, Graphics::bagSlotBorder, Component::Icon_Type::item);
    Mouse::Init_mouse(zone);
    UI_Spellbook::Init_UI(zone);
    Character_Stats::Init_UI(zone, state);
    if (Create_Entities::startup) {
      Character_Stats::Init_Player(zone, state, options);
    } else {
      Recreate_Player(zone, state);
    }
    Maps::Init_Tile_Objects(zone, state);
    Quad_Tree::Fill_Quad_Tree(zone, state);
    Action_Bar::Create_Action_Bar(zone);
  };

  void Restart_Game() {
    Collision::close_collision(World::GameState::overworld);
    Collision::close_collision(World::GameState::cave);
    Maps::close_zone(World::zone);
    Maps::close_zone(World::cave);
    Quad_Tree::quadTrees[World::GameState::overworld].clear();
    Quad_Tree::quadTrees[World::GameState::cave].clear();
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
    Graphics::createGraphicsContext();
    Init_Data();
  }

  void Reload_Zone(entt::registry &zone, World::GameState &state) {
    Game game;
    Recreate_Player(zone, state);
    Init_World(game);
  }

  Game Start_Game(entt::registry &zone, World::GameState &state, Loaded &loaded) {
    Game game;
    if (state == World::GameState::overworld) { loaded.overworld = true; }
    if (state == World::GameState::cave) { loaded.cave = true;}
    if (Create_Entities::startup) {
      options = Character_Create();
      if (!options.success) {
        return game;
      }
    }
    Create_Game_entities(zone, state, options);
    Init_World(game);
    return game;
  }
}// namespace Init
