#pragma once
#include "entt/entt.hpp"

struct Tile {
  bool created = false;
  entt::entity dungeon;
  std::vector<entt::entity> objects;
};

Tile tilesEntities[256][128];

void Init_Tiles_Array() {
    for (int i = 0; i < 255; ++i) {
        for (int j = 0; j < 127; ++j) {
          tilesEntities[i][j].created = false;
        }
    }
}
