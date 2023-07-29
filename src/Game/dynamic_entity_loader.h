#pragma once
#include "entt/entt.hpp"

struct Tile {
  bool created = false;
  std::vector<entt::entity> objects;
};

Tile tilesToRender[256][128];

void Init_Tiles_Array() {
    for (int i = 0; i < 255; ++i) {
        for (int j = 0; j < 127; ++j) {
            tilesToRender[i][j].created = false;
        }
    }
}
