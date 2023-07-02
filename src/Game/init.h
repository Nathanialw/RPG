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
#include "sounds.h"
#include "main_menu.h"


namespace Init {

	std::string batch = "1";


    void Init_World () {
        Collision::init_Collison();
        Init_Zone(World::zone);
        Maps::Create_Map();
        Character_Stats::Init_UI(World::zone);
        Dynamic_Quad_Tree::Fill_Quad_Tree(World::zone);
        UI_Spellbook::init();
        SQLite_Dialogue::Init_Dialogue();
    }


	void Init_Client() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		//	SDL_Log("init error", SDL_GetError());
		}
	//	Joystick::JoystickInit();
		TTF_Init();


		SDL_SetRelativeMouseMode(SDL_TRUE);

		Graphics::running = true;

        Graphics::createGraphicsContext(World::zone);
        Sounds::init();

        Main_Menu::Menu_Options();

        Init_World();
    }




}
