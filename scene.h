#pragma once
#include "entt.hpp"
#include <iostream>
#include "components.h"
#include "graphics.h"
#include "entity_loader.h"
#include "death_spells.h"
#include "world.h"
#include "recruitable_units.h"
#include "unit_control.h"
#include "unit_formations.h"
#include "sprite_parser.h"
#include "collision.h"


using namespace Component;

namespace Scene {

	//create templates of a "type" of entity, like a skeleton
	//import from SQLite with a for loop where it just graps all the template data from tables and the only data I need to set manually is the position with the "potential position" variable. Not sure where to keep the position data so it is editable, maybe a separate file with all the map "tile" data
	//

	//adds Environment items to world grid
	void add_unit_to_grid(Map::Node3& map) {
		auto view = World::zone.view<Position, Radius, Environment>(entt::exclude<Assigned_To_Formation>);
		for (auto entity : view) {
			auto& x = view.get<Position>(entity).x;
			auto& y = view.get<Position>(entity).y;
			auto& r = view.get<Radius>(entity).fRadius;
			SDL_FRect rect = { x - r, y - r, r * 2.0f, r * 2.0f };

			Map::Place_Rect_On_Grid(rect, Map::map, entity);
			World::zone.emplace_or_replace<Assigned_To_Formation>(entity);
		}
	}

	void Spawn_Skeletons(entt::registry &zone, int x, int y) {
		for (float j = 0; j < x; ++j) {
			for (float i = 0; i < y; ++i) {
				Death_Spells::Summon_Skeleton(zone, (100.0f + (i * 60.0f)), (100.0f + (j * 60.0f)), "'skeleton'");
			//	Death_Spells::Summon_Skeleton(zone, (200.0f + (i * 60.0f)), (200.0f + (j * 60.0f)), "'skeleton_mage'");
			}
		}
	}

	//void Emplace_State_Data(entt::entity &entity) {
	//	World::zone.emplace<Store_State>(entity);
	//	//Sprite_Parser::Store_State_Frame_Start_Y();
	//}

	void Update_Army() {
		auto company_view = World::zone.view<Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);
			for (int c = 0; c < company.iSub_Units.size(); c++) {
				auto& platoon = World::zone.get<Platoon>(company.iSub_Units[c]);
				for (int p = 0; p < platoon.iSub_Units.size(); p++) {
					auto& squad = World::zone.get<Squad>(platoon.iSub_Units[p]);
					for (int i = 0; i < squad.iSub_Units.size(); i++) {
						if (squad.bAlive.at(i) == false) {

						}
					}
				}
			}
		}
	}

	void update_scene() {
		//Update_Army();
		//std::cout << "Update_Army = Good" << std::endl;
		//std::cout << "add_terrain_to_grid = Good" << std::endl;
		//add_unit_to_grid(Map::map);
		//std::cout << "add_unit_to_grid = Good" << std::endl;
	}

	void Init_Zone(entt::registry &zone) {
		Map::Build_Map(Map::terrain);
		Map::Build_Map(Map::map);
		Entity_Loader::init_db();
		SDL_RenderSetScale(Graphics::renderer, 2.0f, 2.0f);

	}
}
