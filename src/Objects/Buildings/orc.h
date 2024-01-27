#pragma once
#include "create_entities.h"
#include "mouse_control.h"

namespace Orc {

  std::string buildingType = "buildings_orc";

  void Building_On_Mouse(entt::registry &zone, int &state) {
    //create the basic object on the mouse
    std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
    Collision_Component::aabb aabb;
    Component::Line_Segment line = {};

    //    int xmlIndex = Utilities::Get_Random_Number(1, Game_Objects_Lists::tilesets["buildings_orc"].size() - 1);
    std::string name = "Orc Tent 3 T2";
    int xmlIndex;
    if (Game_Objects_Lists::indexes[buildingType].contains(name)) {
      xmlIndex = Game_Objects_Lists::indexes[buildingType][name];
    } else {
      Utilities::Log(name + " not found");
      return;
    }
    Create_Entities::PVG_Building(zone, state, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, name, xmlIndex, aabb, pointVecs, line);

    //set mouse obect to building

    //when placed add collision box and interaction rect
  }

  void House(entt::registry &zone, int &state) {
    std::string name = "Orc Tent 3 T2";
    int xmlIndex;
    if (Game_Objects_Lists::indexes[buildingType].contains(name)) {
      xmlIndex = Game_Objects_Lists::indexes[buildingType][name];
    } else {
      Utilities::Log(name + " not found");
      return;
    }
    entt::entity entity = Create_Entities::Create_Render_Object(zone, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, name, xmlIndex);
    Create_Entities::Set_On_Mouse(zone, entity);
  }

  void Tent_1(entt::registry &zone, int &state) {
    std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
    Collision_Component::aabb aabb;
    Component::Line_Segment line = {};

    int xmlIndex = Utilities::Get_Random_Number(1, Game_Objects_Lists::tilesets[buildingType].size() - 1);

    Create_Entities::PVG_Building(zone, state, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, Game_Objects_Lists::tilesets[buildingType].at(xmlIndex), xmlIndex, aabb, pointVecs, line);
  }


}// namespace Orc