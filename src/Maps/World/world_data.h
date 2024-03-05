#pragma once
#include "entt/entt.hpp"

namespace World_Data {

  struct object {
    entt::entity entity;
  };

  const int maxObjectsPerTile = 20;

  struct Tile {
    std::array<object, maxObjectsPerTile> objects{};
    entt::entity tileObject{};
    bool isTileObject = false;
    int tileTexture = 0;
    bool created = false;
    float noise{};
  };

  const int REGION_SIZE = 32;
  std::array<Tile[REGION_SIZE][REGION_SIZE], 4> tilesEntities = {};

  const int numZones = 200;//max number of zones


}// namespace World_Data