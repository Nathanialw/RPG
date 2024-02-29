#pragma once
#include "components.h"

namespace Sprite_Parser {

	//std::map<Component::Action_State, int> actionState;



//	void Store_State_Frame_Start_Y(Component::Action_State &state, Component::Store_State &storage, int &y) {
//		//storage.state[state] = y;
//	}


	SDL_Rect Update_Sprite_Sheet(int actionState, int direction) {
		//actionState = column, eact state should have a "y" cord attatched to it, I think use std::map

		// direction = row, should inject an int based on which direction direction

		int width = 96;
		int height = 96;

		SDL_Rect sprite_to_clip = {0 , 0, width, height };
		sprite_to_clip.y = height * direction;
		sprite_to_clip.x = width * actionState;

		return sprite_to_clip;
	}




}