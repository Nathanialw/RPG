#pragma once
#include "Objects/Collision/collision.h"
#include "Objects/Movement/unit_formations.h"
#include "Objects/Stats/character_data.h"
#include "SQLite_unit_data.h"
#include "components.h"
#include "entt/entt.hpp"
#include "graphics.h"
#include "recruitable_units.h"
#include "sprite_parser.h"
#include "texture_packer.h"
#include "unit_control.h"
#include "world.h"
#include <iostream>

namespace Scene {

  //create templates of a "type" of entity, like a skeleton
  //import from SQLite with a for loop where it just graps all the template data from tables and the only data I need to set manually is the position with the "potential position" variable. Not sure where to keep the position data so it is editable, maybe a separate file with all the map "tile" data
  //

  //adds Environment items to world grid
  void add_unit_to_grid(entt::registry &zone, Map::Node3 &map) {
    auto view = zone.view<Component::Position, Component::Radius, Component::Environment>(entt::exclude<Component::Assigned_To_Formation>);
    for (auto entity: view) {
      auto &x = view.get<Component::Position>(entity).x;
      auto &y = view.get<Component::Position>(entity).y;
      auto &r = view.get<Component::Radius>(entity).fRadius;
      SDL_FRect rect = {x - r, y - r, r * 2.0f, r * 2.0f};

      Map::Place_Rect_On_Grid(rect, Map::map, entity);
      zone.emplace_or_replace<Component::Assigned_To_Formation>(entity);
    }
  }

  void Spawn_Skeletons(entt::registry &zone, World::GameState &state, int x, int y) {
    for (float j = 0; j < x; ++j) {
      for (float i = 0; i < y; ++i) {
        //        Death_Spells::Summon_Skeleton(zone, state, (100.0f + (i * 60.0f)), (100.0f + (j * 60.0f)), "'skeleton'");
        //	Death_Spells::Summon_Skeleton(zone, (200.0f + (i * 60.0f)), (200.0f + (j * 60.0f)), "'skeleton_mage'");
      }
    }
  }

  //void Emplace_State_Data(entt::entity &entity) {
  //	zone.emplace_or_replace<Store_State>(entity);
  //	//Sprite_Parser::Store_State_Frame_Start_Y();
  //}

  void Update_Army(entt::registry &zone) {
    auto company_view = zone.view<Component::Company>();
    for (auto companies: company_view) {
      auto &company = company_view.get<Component::Company>(companies);
      for (int c = 0; c < company.iSub_Units.size(); c++) {
        auto &platoon = zone.get<Component::Platoon>(company.iSub_Units[c]);
        for (int p = 0; p < platoon.iSub_Units.size(); p++) {
          auto &squad = zone.get<Component::Squad>(platoon.iSub_Units[p]);
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

  void Init_Zone() {
    Map::Build_Map(Map::terrain);
    Map::Build_Map(Map::map);
    Entity_Loader::init_db();
    Character_Data::Get_Names_From_DB();
  }
}// namespace Scene
