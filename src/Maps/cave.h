#pragma once
#include "components.h"
#include "world.h"

namespace Cave {

  bool Set_As_Cave(entt::registry &zone, entt::entity &entity, std::string &templateName) {
    if (templateName == "CaveEntrance1" ||
        templateName == "CaveEntrance2" ||
        templateName == "CaveEntrance3") {

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