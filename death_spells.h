#pragma once
#include "entt.hpp"
#include "components.h"
#include "entity_loader.h"
#include "collision.h"

namespace Death_Spells {

	/*
	The x and y can be got rid of and the Position components can be updated elswhere; no reason to couple like that here
	*/
	void Summon_Skeleton(entt::registry &zone, float x, float y, std::string name) {

		int unitID = 1;
		const char* filepath = "sprites/units/skeleton/skeleton_mage_00.png";

		Graphics::Create_Game_Object(unitID, filepath);

		int i = rand() % 10 + 1;
		int j = rand() % 10 + 1;
		int k = rand() % 10 + 1;
		int l = rand() % 10 + 1;
		Entity_Loader::Data data = Entity_Loader::parse_data(name);

		auto skeleton0 = zone.create();
		zone.emplace<Component::animation>(skeleton0, Graphics::unitTextures[unitID]); /// need to load the texture nly once and pass the pointer intothis function
		zone.get<Component::animation>(skeleton0).sheet = { //populate the vector
			{ NULL },
			{ {0   , 0, 128, 128}, 0,    512,  1, 0, 75, 0},//idle array[numframes] = { 2ms, 4ms, 2ms}
			{ {512,  0, 128, 128}, 512,  1024, 0, 0, 75, 0},//walk
			{ {1536, 0, 128, 128}, 1536, 512,  0, 0, 75, 0},//atack
			{ {2048, 0, 128, 128}, 2048, 512,  0, 0, 75, 0},//cast
			{ {2560, 0, 128, 128}, 2560, 256,  0, 0, 75, 0},//block
			{ {2816, 0, 128, 128}, 2816, 768,  0, 0, 75, 0}, //reverse to summon
			{ {3584, 0, 128, 128}, 3584, 512,  1, 0, 75, 0},//ranged
		};
		zone.emplace<Component::Sprite_Offset>(skeleton0, 60.0f, 95.0f );
		auto &scale = zone.emplace<Component::Scale>(skeleton0, 1.0f);

		zone.emplace<Component::Actions>(skeleton0, Component::idle);
		zone.get<Component::Actions>(skeleton0).frameCount = { {0, 0}, { 4, 0}, {7, 0}, {4, 0}, {4,0}, {2,0}, {5,0}, {4,0} };

		auto& position = zone.emplace<Component::Position>(skeleton0, x+i-j-k+l, y+j-i-k+l);
		zone.emplace<Component::Radius>(skeleton0, data.radius);
		zone.emplace<Component::Velocity>(skeleton0, 0.f, 0.0f, 0.f, 0.0f, data.speed);

		zone.emplace<Component::Direction>(skeleton0, Component::Direction::SE);
		zone.emplace<Component::handle>(skeleton0, name);
		zone.emplace<Component::Mass>(skeleton0, data.mass);
		
		zone.emplace<Component::Alive>(skeleton0, true);
		zone.emplace<Component::Health>(skeleton0, 30);
		zone.emplace<Component::Melee_Damage>(skeleton0, 2, 5);
		zone.emplace<Component::Attack_Speed>(skeleton0, 2000, 0);

		zone.emplace<Component::Melee_Range>(skeleton0, ((data.radius * 2.0f)));
		zone.emplace<Component::Sight_Range>(skeleton0, position.x - 250.0f, position.y - 250.0f, 500.0f, 500.0f);


		zone.emplace<Component::Soldier>(skeleton0);
		zone.emplace<Component::Commandable>(skeleton0);
		zone.emplace<Component::Spellbook>(skeleton0);
		zone.emplace<Component::Entity_Type>(skeleton0, Component::Entity_Type::unit);
		
		bool yes = true;
		Collision::Create_Dynamic_Body(zone, skeleton0, position.x, position.y, data.radius, data.mass, yes);
	}

}