#pragma once

#include "ai_control.h"
#include "character_stats.h"
#include "collision.h"
#include "debug_system.h"
#include "event_handler.h"
//#include "formation_collisions.h"
#include "graphics.h"
#include "init.h"
#include "interface.h"
//#include "map.h"
#include "movement.h"
#include "rendering.h"
#include "sounds.h"
#include "spells.h"
//#include "squad_control.h"
#include "texture_packer.h"
//#include "ui_gameloop_function_times.h"
//#include "unit_positions.h"
//#include "unit_status.h"
#include <SDL2/SDL.h>
#include "quad_tree.h"

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

      Event_Handler::Update_User_Input(zone, state);
      Character_Stats::Update_Items(zone, state);
      Player_Control::Move_To_Atack_Routine(zone,state);
      AI::Update_AI(zone, state);
      Spells::Update_Spells(zone, state);
      Combat_Control::Update_Attacks(zone, state);
      Movement::Update_Entity_Positions(zone);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::movement], Timer::gameLoopTimer);

      Collision::Collision_Routine(zone, state);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::collision], Timer::gameLoopTimer);

      Unit_Status::Update_Unit_Status(zone, state);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::status], Timer::gameLoopTimer);

      if (!Rendering::Rendering(zone, state)) {
        //restart
        return 1;
      }
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::render], Timer::gameLoopTimer);

      Quad_Tree::Update_Tree_Routine(zone, state);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::update_quad_tree], Timer::gameLoopTimer);

      Rendering::Present();
      Timer::Calculate_Timestep();
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::renderpresent], Timer::gameLoopTimer);


      if (World::currentZone.current != World::currentZone.next) { return World::currentZone.next;}
    }
    return state;
  }

  bool Game_Running(entt::registry &zone, int &state) {
    Game_State();
    Timer::startPerf = SDL_GetPerformanceCounter();

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
