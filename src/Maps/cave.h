#pragma once
#include "components.h"
#include "world.h"

namespace Cave {

  bool Set_As_Cave(entt::registry &zone, entt::entity &entity, std::string &templateName) {
    if (templateName == "Rock_3_1" ||
        templateName == "Rock_3_2" ||
        templateName == "Rock_3_3" ||
        templateName == "Rock_3_4" ||
        templateName == "Rock_3_5" ||
        templateName == "Rock_3_6" ||
        templateName == "Rock_3_7" ||
        templateName == "Rock_3_8" ||
        templateName == "Rock_2_1" ||
        templateName == "Rock_2_2" ||
        templateName == "Rock_2_3" ||
        templateName == "Rock_2_4" ||
        templateName == "Rock_2_5" ||
        templateName == "Rock_2_6" ||
        templateName == "Rock_2_7" ||
        templateName == "Rock_2_8") {

      World::increment_Zone();
      auto &dungeon = zone.emplace_or_replace<Component::Dungeon>(entity);
      dungeon.instance = World::Zone_Count;
      dungeon.tilesetName = "hell";
      zone.emplace_or_replace<Component::Entity_Type>(entity, Component::Entity_Type::portal);
      return true;
    }
    return false;
  }

  void Surface(int &state) {
    World::currentZone.next = 2;
    World::world[World::currentZone.next].previousZoneIndex = state;
  }

  void Load_Zone(entt::registry &zone, entt::entity &targetID, int &state) {
    //get the new zone index
    auto newZone = zone.get<Component::Dungeon>(targetID).instance;
    //save the old index go back to it later
    World::world[newZone].previousZoneIndex = state;
    World::world[newZone].tilesetName = "hell";
    World::currentZone.next = newZone;
    Clear_Tiles_Array(zone);
  }

}// namespace Cave