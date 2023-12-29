#pragma once
#include "entt/entt.hpp"

struct object {
  entt::entity entity;
//  bool exists = false;
};

struct Tile {
  std::array<object, 4> objects;
  entt::entity dungeon;
  bool created = false;
};

const int REGION_SIZE = 32;
std::array<Tile[REGION_SIZE][REGION_SIZE], 4> tilesEntities;

void Init_Tiles_Array() {
  for (int i = 0; i < REGION_SIZE; ++i) {
    for (int j = 0; j < REGION_SIZE; ++j) {
      tilesEntities[0][i][j].created = false;
    }
  }
}
