#pragma once

#include "Objects/Collision/collision.h"
#include "Objects/Stats/character_stats.h"
#include "ai_control.h"
#include "debug_system.h"
#include "event_handler.h"
#include "graphics.h"
#include "init.h"
#include "interface.h"
#include "Graphics/Rendering/rendering.h"
#include "Input/Movement/movement.h"
#include "effects.h"
#include "spells.h"
#include "Graphics/XML_Parsers/texture_packer.h"
#include "Audio/init.h"
#include "Maps/Pathing/a_star.h"
#include "quad_tree.h"
#include "update_spells.h"
#include <SDL2/SDL.h>

namespace Game_Loop {

  void Game_State() {
    if (!Graphics::running) {
      World::gamestate = World::GameState::exit;
    }
  }

  int Game_Loop(entt::registry &zone, int &state) {
    World::currentZone = {state, state};
    while (state != 0) {
      Game_State();
      //Squad_Control::Create_And_Fill_New_Squad(zone);
      //Test_Units::Create_Formation(zone);
      Sound::Update_Volume();
      //ideally this would call on init and set all indexes with an object as an obstacle and only update by the object that gets moved
      A_Star::Update(zone);

      Event_Handler::Update_User_Input(zone, state);
      Create_Character_Entity::Update_Items(zone, state);
      Player_Control::moveToAttackRoutine(zone, state);
      AI::Update_AI(zone, state);
      Update_Spells::Update_Spells(zone, state);
      Combat_Control::Update_Attacks(zone, state);

      Movement::Update_Entity_Positions(zone);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::movement], Timer::gameLoopTimer);

      Collision::Collision_Routine(zone, state);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::collision], Timer::gameLoopTimer);

      Unit_Status::Update_Unit_Status(zone, state);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::status], Timer::gameLoopTimer);

      if (World::currentZone.current == World::currentZone.next) {
        if (!Rendering::Rendering(zone, state)) {
          //restart
          return 1;
        }
      }
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::render], Timer::gameLoopTimer);

      if (World::currentZone.current != World::currentZone.next) { World_Update::Clear_Tiles_Array(zone); }
      Quad_Tree::Update_Tree_Routine(zone, state);

      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::update_quad_tree], Timer::gameLoopTimer);

      Rendering::Present();
      Timer::Calculate_Timestep();
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::renderpresent], Timer::gameLoopTimer);
      if (World::currentZone.current != World::currentZone.next) { return World::currentZone.next; }
    }
    return state;
  }

  bool Game_Running(entt::registry &zone, int &state) {
    Game_State();
    Timer::startPerf = SDL_GetPerformanceCounter();
    Interface::Update_Zoom(zone, -1);

    state = Game_Loop(zone, state);

    return state;
    //
    //    switch (World::gamestate) {
    //      case World::GameState::overworld: { return World::GameState::overworld; break;}
    //        //need a cave init function
    //      //need collision bodies removed from the overworld first
    //      //need object tree copied and a different one for the cave from the overworld first
    //      case World::GameState::restart: {return World::GameState::restart; break;}
    //      case World::GameState::exit: {return World::GameState::exit; break;}
    //      case World::GameState::cave: {return World::GameState::cave; break;}
    //      case World::GameState::MODES: {return World::GameState::exit; break;}
    //    }
  }
}// namespace Game_Loop
