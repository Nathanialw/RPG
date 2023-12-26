#pragma once
#include "create_entities.h"
#include "components.h"

/* array of blood names to pick a random one*/

namespace Blood {

  void Splatter(entt::registry &zone, int &state, Component::Position position) {

    std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
    Collision::aabb aabb;
    Component::Line_Segment line = {};

    int splatterIndex = Utilities::Get_Random_Number(1, Game_Objects_Lists::tilesets["bloodSplatter"].size() - 1);

    Create_Entities::PVG_Building(zone, state, position.x, position.y, position.x, position.y, Game_Objects_Lists::tilesets["bloodSplatter"][splatterIndex], splatterIndex, aabb, pointVecs, line);
  }

  void Pool(entt::registry &zone, int &state, Component::Position position) {

    std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
    Collision::aabb aabb;
    Component::Line_Segment line;


    int poolIndex = Utilities::Get_Random_Number(1, Game_Objects_Lists::tilesets["bloodPool"].size() - 1);
    Create_Entities::PVG_Building(zone, state, position.x, position.y, position.x, position.y, Game_Objects_Lists::tilesets["bloodPool"][poolIndex], poolIndex, aabb, pointVecs, line);
  }

  void Import_Blood_Sprites() {
    // read in db data
    // build xml and save spritesheet into the graphics map
    // spawn blood as an enii:entity when a unit dies
    // render blood in the background objects render layer
  }
};