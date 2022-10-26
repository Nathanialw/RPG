#pragma once
#include "tinyxml2.h"
#include "sprite_sheets.h"

namespace Texture_Packer {

	
	void Parse_Doc() {
		tinyxml2::XMLDocument spriteSheetData;
		const char* path = "sprites/units/animals/can_Die/lion_male/lion/lion_male.xml";

		spriteSheetData.LoadFile(path);
		tinyxml2::XMLElement* pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");
		
		Component::Sprite_Sheet_Data frame{};
		int i = 0;
		while (pSpriteElement != NULL) {
			//assign value
			frame.name = pSpriteElement->Attribute("n");
			frame.clip.x = pSpriteElement->IntAttribute("x");
			frame.clip.y = pSpriteElement->IntAttribute("y");
			frame.clip.w = pSpriteElement->IntAttribute("w");
			frame.clip.h = pSpriteElement->IntAttribute("h");
			frame.offset.x = pSpriteElement->IntAttribute("oX");
			frame.offset.y = pSpriteElement->IntAttribute("oY");
			Sprite_Sheet::maleLionSheet[i] = frame;
			i++;
			//getNext
			pSpriteElement = pSpriteElement->NextSiblingElement("sprite");			
		}
		
		Sprite_Sheet::maleLionActionIndex[(int)Action_State::attack] = { 0, 9 };
		Sprite_Sheet::maleLionActionIndex[(int)Action_State::struck] = { 71, 9 };
		Sprite_Sheet::maleLionActionIndex[(int)Action_State::cast] = { 143, 7 };//roaring 143
		//running 199
		//sit 271
		Sprite_Sheet::maleLionActionIndex[(int)Action_State::idle] = { 279, 1 };//stop 279 //idle has no directions?
		Sprite_Sheet::maleLionActionIndex[(int)Action_State::dead] = { 287, 11 };//die 287
		Sprite_Sheet::maleLionActionIndex[(int)Action_State::walk] = { 375, 8 };//walk 375




	}





	

}