#pragma once
#include "world.h"

namespace Cave {

  bool Cave = false;
  bool Overworld = false;

  void Enter_Cave(entt::registry &zone, entt::entity &targetID, int &state) {
    //get the new zone index
    auto newZone = zone.get<Component::Dungeon>(targetID).instance;
    //save the old index go back to it later
    World::world[newZone].previousZoneIndex = state;
  }

  void Surface(int &state) {
    state = 2;
  }

  void Load_Zone(entt::registry &zone, entt::entity &targetID, int &state) {
    //get the new zone index
    auto newZone = zone.get<Component::Dungeon>(targetID).instance;
    //save the old index go back to it later
    World::world[newZone].previousZoneIndex = state;
    World::world[newZone].tilesetName = "cave";
    World::currentZone.next = newZone;
  }

  //hide overworld move input character to new location?
  //set tile type to render
  //set unit types to create
}