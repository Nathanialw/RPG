#pragma once
#include "components.h"
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
std::array<Tile[REGION_SIZE][REGION_SIZE], 4> tilesEntities = {};

void Clear_Tiles_Array(entt::registry &zone) {
  for (int i = 0; i < REGION_SIZE; ++i) {
    for (int j = 0; j < REGION_SIZE; ++j) {
      if (tilesEntities[0][i][j].created) {
        for (auto objects: tilesEntities[0][i][j].objects) {
          if (zone.any_of<Component::Interaction_Rect>(objects.entity)) {
            auto &rect = zone.get<Component::Interaction_Rect>(objects.entity);
            zone.emplace_or_replace<Component::Remove_From_Object_Tree>(objects.entity, rect.rect);
          }
        }
      }
    }
  }
}

void Init_Tiles_Array() {
  for (int i = 0; i < REGION_SIZE; ++i) {
    for (int j = 0; j < REGION_SIZE; ++j) {
      tilesEntities[0][i][j].created = false;
    }
  }
}

void Check_Quadrant(Component::Camera &camera, int &x, int &y) {
  int posx = REGION_SIZE / 4;
  int posy = REGION_SIZE / 4;
}


//check which quadrant player is in
//load the 4 zones adjacet to the quadrant of the zone you are in
