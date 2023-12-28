#pragma once
#include "world.h"
#include "components.h"

namespace Cave {

  void Surface(int &state) {
    World::currentZone.next = 2;
    World::world[World::currentZone.next].previousZoneIndex = state;
  }

  void Load_Zone(entt::registry &zone, entt::entity &targetID, int &state) {
    //get the new zone index
    auto newZone = zone.get<Component::Dungeon>(targetID).instance;
    //save the old index go back to it later
    World::world[newZone].previousZoneIndex = state;
    World::world[newZone].tilesetName = "cave";
    World::currentZone.next = newZone;
  }

}