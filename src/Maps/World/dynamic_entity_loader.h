#pragma once
#include "Procedural_Generation/simplex_noise.h"
#include "components.h"
#include "entt/entt.hpp"

struct object {
  entt::entity entity;
};

const int maxObjectsPerTile = 5;

struct Tile {
  std::array<object, maxObjectsPerTile> objects{};
  bool created = false;
  float noise;
};

const int REGION_SIZE = 32;
std::array<Tile[REGION_SIZE][REGION_SIZE], 4> tilesEntities = {};

void Clear_Tiles_Array(entt::registry &zone) {
  for (int i = 0; i < REGION_SIZE; ++i) {
    for (int j = 0; j < REGION_SIZE; ++j) {
      if (tilesEntities[0][i][j].created) {
        for (auto object: tilesEntities[0][i][j].objects) {
          if (zone.any_of<Component::Interaction_Rect>(object.entity)) {
            auto &rect = zone.get<Component::Interaction_Rect>(object.entity);
            zone.emplace_or_replace<Component::Remove_From_Object_Tree>(object.entity, rect.rect);
          }
        }
      }
    }
  }
}

void Init_Tiles_Array() {
  Simplex_Noise::Init(REGION_SIZE, REGION_SIZE);
  Simplex_Noise::Generate_Map(REGION_SIZE, REGION_SIZE);

  for (int i = 0; i < REGION_SIZE; ++i) {
    for (int j = 0; j < REGION_SIZE; ++j) {
      tilesEntities[0][i][j].created = false;
      tilesEntities[0][i][j].noise = Simplex_Noise::fPerlinNoiseArray2D[(i * REGION_SIZE) + j];
    }
  }
}


//check which quadrant player is in
//load the 4 zones adjacent to the quadrant of the zone you are in

//void Check_Quadrant(Component::Camera &camera, int &x, int &y) {
//  int posx = REGION_SIZE / 4;
//  int posy = REGION_SIZE / 4;
//}
