#pragma once
#include "Pathing/pathing_map.h"
#include "Procedural_Generation/labyrinth.h"
#include "Procedural_Generation/simplex_noise.h"
#include "components.h"
#include "entt/entt.hpp"
#include "world_data.h"

namespace World_Update {

  void Clear_Tiles_Array(entt::registry &zone) {
    for (int i = 0; i < World_Data::REGION_SIZE; ++i) {
      for (int j = 0; j < World_Data::REGION_SIZE; ++j) {
        if (World_Data::tilesEntities[0][i][j].created) {
          for (auto object: World_Data::tilesEntities[0][i][j].objects) {
            if (zone.any_of<Component::Interaction_Rect>(object.entity)) {
              auto &rect = zone.get<Component::Interaction_Rect>(object.entity);
              zone.emplace_or_replace<Component::Remove_From_Object_Tree>(object.entity, rect.rect);
            }
          }
        }
      }
    }
  }

  void Init_Tiles_Array(entt::registry &zone, int state) {
    Simplex_Noise::Init(World_Data::REGION_SIZE, World_Data::REGION_SIZE);
    Simplex_Noise::Generate_Map(World_Data::REGION_SIZE, World_Data::REGION_SIZE);
    Labyrinth::Init(state);
    Labyrinth::Generate_Map();
    A_Star::Init();
    //    A_Star::Update(zone);
    if (World::world[state].tileset == "labyrinth") {
      Pathing_Map::Labyrinth(Labyrinth::labyrinth);
    }

    for (int i = 0; i < World_Data::REGION_SIZE; ++i) {
      for (int j = 0; j < World_Data::REGION_SIZE; ++j) {
        World_Data::tilesEntities[0][i][j].created = false;
        World_Data::tilesEntities[0][i][j].noise = Simplex_Noise::fPerlinNoiseArray2D[(j * World_Data::REGION_SIZE) + i];
        World_Data::tilesEntities[0][i][j].tileTexture = Labyrinth::labyrinth[(j * World_Data::REGION_SIZE) + i];
      }
    }
  }


  //check which quadrant player is in
  //load the 4 zones adjacent to the quadrant of the zone you are in

  //void Check_Quadrant(Component::Camera &camera, int &x, int &y) {
  //  int posx = REGION_SIZE / 4;
  //  int posy = REGION_SIZE / 4;
  //}
}// namespace World_Update