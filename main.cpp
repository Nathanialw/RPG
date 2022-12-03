#include <SDL2/SDL.h>
#include "graphics.h"
#include "rendering.h"
#include "init.h"
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

int main(int argc, char* argv[]) {

	srand((unsigned int)time(NULL));
	//initialization
	Collision::init_Collison();
	Init::init();
	Graphics::createGraphicsContext(World::zone);
    Sounds::init ();
	Init_Zone(World::zone);
	update_scene(); //tries to add new environment objects and terrain the the world grid every frame
	Maps::Create_Map();
	Character_Stats::Init_UI(World::zone);
	Dynamic_Quad_Tree::Fill_Quad_Tree(World::zone);
    UI_Spellbook::init();

	while (Graphics::running == true) {

		//Squad_Control::Create_And_Fill_New_Squad(World::zone);
		//Test_Units::Create_Formation(World::zone);


		Event_Handler::Update_User_Input(World::zone);
        Character_Stats::Update_Items(World::zone);

        UI::Move_To_Item_Routine(World::zone, Mouse::itemCurrentlyHeld);
        Death_Control::Dead_Entity_Routine (World::zone);
        Player_Control::Move_To_Atack_Routine(World::zone);

		//std::cout << "Player_Input = Good" << std::endl;
		AI::Update_AI(World::zone);

		//std::cout << "Update_Attacks = Good" << std::endl;
		Spells::Update_Spells();

        //std::cout << "AI = Good" << std::endl;
        Combat_Control::Update_Attacks(World::zone);


        //std::cout << "Update_Spells = Good" << std::endl;
		Movement::Update_Entity_Positions(World::zone);

		//std::cout << "Movement_Handler = Good" << std::endl;
		Collision::Collision_Routine(World::zone);

	//	Unit_Position::Update_Formation_Positions(World::zone);

		//std::cout << "Update_Formation_Rects = Good" << std::endl;

		//Formation_Collision::Test_Collision(World::zone);

		//std::cout << "Collisions = Good" << std::endl;
		Unit_Status ::Update_Unit_Status(World::zone);

		//std::cout << "Update_Unit_Status = Good" << std::endl;
		Rendering::Rendering(World::zone);
        Dynamic_Quad_Tree::Update_Tree_Routine(World::zone);


        Rendering::Present();
		//std::cout << "Rendering = Good" << std::endl;
		Timer::Calculate_Timestep();
		Timer::Calculate_FPS();
		//std::cout << "frameTime = Good" << std::endl;
	}
	//close
	Graphics::closeContext();

	return 0;
}
