#pragma once

#include "Game/Game_Start/create_character_entity.h"
#include "Graphics/Rendering/rendering.h"
#include "Graphics/XML_Parsers/texture_packer.h"
#include "Input/Movement/movement.h"
#include "Maps/World/map.h"
#include "Maps/World/world.h"
#include "Maps/World/world_update.h"
#include "Objects/Collision/collision.h"
#include "Objects/Collision/formation_collisions.h"
#include "Objects/Movement/unit_positions.h"
#include "Objects/Stats/character_stats.h"
#include "Objects/Stats/unit_status.h"
#include "UI/Debug_Panel/ui_debug.h"
#include "ai_control.h"
#include "debug_system.h"
#include "event_handler.h"
#include "graphics.h"
#include "interface.h"
#include "load_object_list.h"
#include "main_menu.h"
#include "menu.h"
#include "spells.h"
#include "squad_control.h"
#include "ui_frames.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Init {

  std::string batch = "1";

  struct Game {
    bool success = false;
    Menu::Menu menu;
    int instanceCount = 0;
    bool overworld = false;
    bool cave = false;

    void incrementInstance() {
      instanceCount++;
    };
  };

  struct Loaded {
    int instance = 2;
  };

  void Clear_Events() {
    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
  }

  Character_Options::Customization Character_Create() {
    return Main_Menu::Menu_Options();
  }

  Character_Options::Customization options;

  void Recreate_Player(entt::registry &zone, int &state) {
    Create_Character_Entity::Recreate_Player(zone, state, options);
  }

  void Init_Data() {
    Graphics::Load_Textures();
    UI_Info::Update_Position();
    UI_Debug::Update_Position();
    Icons::Load();
    UI_Spellbook::Init_UI();
    Action_Bar::Create_Action_Bar();
    Hotbar::Init_Hotbar();
    Scene::Init_Zone();
    Debug::Load_Settings();
    UI_Frames::Load_Buildings();
    //this one should be different for whichever class you create
    Load_Object_List::Load_Entities();
    Menu::Init();
    Menu_Options::Load_Options();
    SQLite_Dialogue::Init_Dialogue();
    Create_Character_Entity::Init_Items();
  }

  //only needs to be fired once per zone
  void Create_Game_entities(entt::registry &zone, int &state, std::string &tilesetName, Character_Options::Customization &playerOptions) {
    Maps::Generate_Region(state, tilesetName);
    Collision::init_Collison(state);
    Item_Component::emptyEquipSlot[state] = Graphics::Create_Icon_Entity(zone, Graphics::emptyBagIcon, Graphics::emptyBagIcon, Component::Icon_Type::item);
    Graphics::defaultIcon[state] = Graphics::Create_Icon_Entity(zone, Graphics::default_icon, Graphics::bagSlotBorder, Component::Icon_Type::item);
    Bag_UI::emptyBagSlot[state] = Graphics::Create_Icon_Entity(zone, Graphics::emptyBagIcon, Graphics::emptyBagIcon, Component::Icon_Type::item);
    Mouse::Init_mouse(zone);
    if (Create_Entities::startup) {
      Create_Character_Entity::Init_Player(zone, state, playerOptions);
    } else {
      Recreate_Player(zone, state);
    }
    Maps::Init_Tile_Objects(zone, state, World::world[state].mobType);
    Maps::Init_Caves(zone, state, World::world[state].cave);
    World_Update::Init_Tiles_Array(zone, state);
    Quad_Tree::Fill_Quad_Tree(zone, state);
  };

  void Restart_Game() {
    for (int i = 0; i < World::world.size(); ++i) {
      Collision::close_collision(i);
      Maps::close_zone(World::world[i].zone);
      Quad_Tree::quadTrees[i].clear();
    }
    World::Zone_Count = 2;
    World::world.clear();
    World::world.resize(World_Data::numZones);
  }

  Game Init_World(Game &game, int &state) {
    Clear_Events();
    Music::Play(World::world[state].music.c_str());
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

  void Reload_Zone(entt::registry &zone, int &state) {
    World_Update::Init_Tiles_Array(zone, state);
    Game game;
    Recreate_Player(zone, state);
    Init_World(game, state);
  }

  Game Start_Game(entt::registry &zone, std::string &tilesetName, int &state) {
    Game game;
    World::world[state].loaded = true;
    if (Create_Entities::startup) {
      options = Character_Create();
      if (!options.success) {
        return game;
      }
    }
    Create_Game_entities(zone, state, tilesetName, options);
    Init_World(game, state);
    return game;
  }
}// namespace Init
