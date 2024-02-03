#pragma once
#include "create_entities.h"
#include "mouse_control.h"

namespace Orc {

  void Building_On_Mouse(entt::registry &zone, int &state) {
    std::string buildingType = "buildings_orc";
    std::string name = "Orc Tent 3 T2";
    int xmlIndex;
    if (Game_Objects_Lists::indexes[buildingType].contains(name)) {
      xmlIndex = Game_Objects_Lists::indexes[buildingType][name];
    } else {
      Utilities::Log(name + " not found");
      return;
    }
    Create_Entities::PVG_Building(zone, state, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, name, xmlIndex);
  }

  void House(entt::registry &zone, int &state, std::string name) {
    if (Mouse_Struct::mouseData.type == Component::Icon_Type::none) {
      int xmlIndex = -1;
      auto mouseEntity = Create_Entities::Create_Render_Object(zone, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, name, xmlIndex);
      Mouse_Struct::mouseData.name = mouseEntity.name;
      Create_Entities::Set_On_Mouse(zone, mouseEntity);
    }
  }

  void Tent_1(entt::registry &zone, int &state) {
    std::string buildingType = "buildings_orc";
    int xmlIndex = Utilities::Get_Random_Number(1, Game_Objects_Lists::tilesets[buildingType].size() - 1);
    Create_Entities::PVG_Building(zone, state, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, Game_Objects_Lists::tilesets[buildingType].at(xmlIndex), xmlIndex);
  }
}// namespace Orc