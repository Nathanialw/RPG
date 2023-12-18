#pragma once
#include "ai_control.h"
#include "character_stats.h"
#include "collision.h"
#include "debug_system.h"
#include "event_handler.h"
#include "formation_collisions.h"
#include "graphics.h"
#include "init.h"
#include "interface.h"
#include "map.h"
#include "movement.h"
#include "rendering.h"
#include "sounds.h"
#include "spells.h"
#include "squad_control.h"
#include "texture_packer.h"
#include "ui_gameloop_function_times.h"
#include "unit_positions.h"
#include "unit_status.h"
#include <SDL2/SDL.h>

namespace Game_Loop {

  bool running = false;

  void Game_State() {
    if (Graphics::running == false) {
      running = false;
    } else {
      running = true;
    }
  }

  void Game_Loop() {
    Game_State();
    Timer::startPerf = SDL_GetPerformanceCounter();
    while (running == true) {
      Game_State();
      //Squad_Control::Create_And_Fill_New_Squad(World::zone);
      //Test_Units::Create_Formation(World::zone);

      Event_Handler::Update_User_Input(World::zone);
      Character_Stats::Update_Items(World::zone);
      Player_Control::Move_To_Atack_Routine(World::zone);
      AI::Update_AI(World::zone);
      Spells::Update_Spells();
      Combat_Control::Update_Attacks(World::zone);
      Movement::Update_Entity_Positions(World::zone);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::movement], Timer::gameLoopTimer);

      Collision::Collision_Routine(World::zone);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::collision], Timer::gameLoopTimer);

      Unit_Status::Update_Unit_Status(World::zone);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::status], Timer::gameLoopTimer);

      Rendering::Rendering(World::zone);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::render], Timer::gameLoopTimer);

      Dynamic_Quad_Tree::Update_Tree_Routine(World::zone);
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::update_quad_tree], Timer::gameLoopTimer);

      Rendering::Present();
      Timer::Calculate_Timestep();
      Update_Game_Loop_Timers(Timer::GameStateValue[Timer::renderpresent], Timer::gameLoopTimer);
    }
  }
}// namespace Game_Loop
