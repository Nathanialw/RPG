#pragma once

#include "entt/entt.hpp"
#include "graphics.h"
#include "procedural_generator.h"
#include "create_entities.h"
#include "game_objects.h"
#include <map>
#include "dynamic_entity_loader.h"

namespace World_Data {
  SDL_Texture *tileTextures[200];

  struct Tile_Objects {
    entt::entity objects[4];
  };

  //  std::array<std::array<bool, 64>, 64> Map;

  struct Tile_Size {
    float width = 512.0f;
    float height = 512.0f;
  };


  struct Offset {
    float x;
    float y;
  };

  struct Tile_Position {
    float x;
    float y;
  };

  SDL_Texture *tiles[75];

  enum Tile_Type {
    grass,
    sand,
    dirt
  };

  struct TILE {
    int type;
    bool generated;
    SDL_FRect position;
  };

  TILE Game_Map[256][256];

  enum Area_Type {
    wooded,
    plains,
    town
  };

  enum Zone_Type {
    desert,
    forest,
    ocean
  };

  static const int width = 96;
  static const int height = 96;

  struct Tile {
    //        position
    //        float x;
    //        float y;
    bool hh;
  };

  struct Region {
    Tile board[width][height];
  };

  //    std::vector<Zone> World;
}

namespace World {
  World_Data::Offset worldOffset;
  World_Data::Tile_Size size;

  entt::basic_registry<entt::entity> zone;
  World_Data::Region region;

  void Init_Tiles() {
    //        randomize
    worldOffset = {-100.0f, -100.0f};

    World_Data::tiles[0] = Graphics::createTexture("assets/sprites/environment/forest/forest_floor/Grass_Forest_0.png");
    World_Data::tiles[1] = Graphics::createTexture("assets/sprites/environment/forest/forest_floor/Grass_Forest_1.png");
    World_Data::tiles[2] = Graphics::createTexture("assets/sprites/environment/forest/forest_floor/Grass_Forest_2.png");
    World_Data::tiles[3] = Graphics::createTexture("assets/sprites/environment/forest/forest_floor/Grass_Forest_3.png");
    World_Data::tiles[4] = Graphics::createTexture("assets/sprites/environment/forest/forest_floor/Grass_Forest_4.png");
  }

  void Create_Map(World_Data::Offset offset) {
    int h = 0;

    for (int w = offset.x; w < (offset.x + World_Data::width); w++) {
      for (int i = offset.y; i < (offset.x + World_Data::height); i++) {
        World_Data::Tile tile = region.board[w][i];

        entt::entity entity = zone.create();
        //get tile position
        World_Data::Tile_Position position;

        position.x = w * size.width;
        position.y = i * size.height;

        //set tile data
        Procedural_Components::Seed seed = zone.emplace<Procedural_Components::Seed>(entity);
        seed.seed = Procedural_Generation::Create_Initial_Seed(position.x, position.y);
        h++;
      }
    }
    Utilities::Log("----MAP SIZE----");
    Utilities::Log(h);
  }

  //    needs to run when the zone is in range
  void Generate_Region() {
    Init_Tiles();

    //go through all tiles and and ust the position to generate a random number, use that number to determine what objects are on each tile
    //        Create_Map({0.0f, 0.0f});

  }

  void Generate() {
    //generate objects only the tiles around the player

    //plug that in as the index ranges
    int w;
    int i;

    int z;
    int j;

    int a;
    int k;

    World_Data::Tile tile = region.board[w][i];
    World_Data::Tile_Position position;
    position.x = ((w * z) + a) * size.width;
    position.y = ((i * j) + k) * size.height;
    //                                auto entity = zone.create();
    //                                zone.emplace<World_Data::Tile>(entity, tile.x, tile.y);
    //                                    takes in tile coords as seed, outputs random number

    Procedural_Components::Seed seed;
    seed.seed = Procedural_Generation::Create_Initial_Seed(position.x, position.y);

    int treesExist = Procedural_Generation::Random_Int(0, 10, seed);

    //        trees
    //        rocks
    //        plant objects

    //small monster camps
    //large monster camps
    //massive monster camps

    //        town
    //        tower

    //      house
    //        farm
  }

  void Get_Coords(int x, int &w) {
    x -= size.width;
    w = x / size.width;
  }

  void Get_Size() {

  }

  std::vector<bool> tiles = {};

  void Renders(Component::Camera &camera) {
    //generate objects only the tiles around the player
    //plug that in as the index ranges
    int x = 0;
    int y = 0;
    Get_Coords(camera.screen.x, x);
    Get_Coords(camera.screen.y, y);

    for (int i = x; i <= (x + ((camera.screen.w + size.width + size.width) / size.width)); i++) {
      for (int j = y; j <= (y + ((camera.screen.h + size.height + size.height) / size.height)); j++) {
        if (i < 0 || j < 0 || j > 127 || i > 255) {
          continue;
        }
        SDL_FRect tile = World_Data::Game_Map[i][j].position;

        SDL_FRect renderRect;
        renderRect.x = tile.x - camera.screen.x;
        renderRect.y = tile.y - camera.screen.y;
        renderRect.w = size.width;
        renderRect.h = size.height;

        SDL_Texture *texture = World_Data::tiles[World_Data::Game_Map[i][j].type];
        SDL_RenderCopyF(Graphics::renderer, texture, NULL, &renderRect);
      }
    }
  }

  void Generate_Map() {
    int x = 0;
    int y = 0;

    for (int i = 0; i <= 255; i++) {
      for (int j = 0; j <= 127; j++) {
        // generate tile type
        // Generate_Trees(SDL_FRect rect);
        Procedural_Components::Seed seed;
        seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
        int textureArraySize = 5;

        World_Data::TILE tile;
        SDL_FRect rect;
        rect.x = ((i + x) * size.width);
        rect.y = (j * size.height);
        rect.w = size.width;
        rect.h = size.height;

        tile.position = rect;
        tile.generated = true;
        tile.type = (World_Data::Tile_Type) Procedural_Generation::Random_Int(0, textureArraySize, seed);

        World_Data::Game_Map[i][j] = tile;
        //Generate_Trees(tile.position);
      }
    }
  }

  void Init_Tile_Objects() {
    float x = 0.0f;
    float y = 0.0f;
    Procedural_Components::Seed seed;

    for (int i = 0; i < 255; ++i) {
      for (int j = 0; j < 127; ++j) {
        seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
        int numObjects = (World_Data::Tile_Type) Procedural_Generation::Random_Int(0, 100, seed);
        if (numObjects < 3) {
          x = (World_Data::Tile_Type) Procedural_Generation::Random_Int(0, (int) size.width, seed);
          y = (World_Data::Tile_Type) Procedural_Generation::Random_Int(0, (int) size.height, seed);

          int n = Procedural_Generation::Random_Int(1, Game_Objects_Lists::beastUnitVec.size() - 1, seed);
          db::Unit_Data data = Game_Objects_Lists::beastUnitVec[n];
          Create_Entities::Create_Entity(World::zone, (i * size.width) + x, (j * size.height) + y, data.name, "", false, data.imgPath, false);
        }
      }
    }
  }

  //    The first time the tile is rendered generate objects but only the first time
  int ii = 0;

  //position of tile
  void Generate_Trees(SDL_FRect rect, Tile &tile) {
    Procedural_Components::Seed seed;
//    collisionBox;
    Collision::aabb aabb;
    tmx::Vector2<float> offset = {0.0f, 0.0f};
    std::vector<std::vector<tmx::Vector2<float>>> pointVecs = {};
    Component::Line_Segment line = {};
    float x = 0.0f;
    float y = 0.0f;
    int xmlIndex = 0;
    std::string objectName = "";

    seed.seed = Procedural_Generation::Create_Initial_Seed(rect.x, rect.y);
    int numObjects = 2;//(World_Data::Tile_Type) Procedural_Generation::Random_Int(4, 10, seed);

    for (int k = 0; k < numObjects; k++) {
      x = (World_Data::Tile_Type) Procedural_Generation::Random_Int(0, (int) size.width, seed);
      y = (World_Data::Tile_Type) Procedural_Generation::Random_Int(0, (int) size.height, seed);

      float i = rect.x / size.width;
      float j = rect.y / size.height;

      xmlIndex = Procedural_Generation::Random_Int(1, Game_Objects_Lists::forestObjectVec.size() - 1, seed);
      objectName = Game_Objects_Lists::forestObjectVec[xmlIndex];
      Create_Entities::PVG_Building(World::zone, rect.x + x, rect.y + y, i, j, objectName, xmlIndex, aabb, pointVecs, line, offset);
    }
  }

  void Render(Component::Camera &camera) {
    int x = 0;
    int y = 0;

    Get_Coords(camera.screen.x, x);
    Get_Coords(camera.screen.y, y);

    for (int i = x; i <= (x + ((camera.screen.w + size.width + size.width) / size.width)); i++) {
      for (int j = y; j <= (y + ((camera.screen.h + size.height + size.height) / size.height)); j++) {
        if (i > 0 || j > 0) {
          // generate tile type
          Procedural_Components::Seed seed;
          seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
          int textureArraySize = 5;
          World_Data::Tile_Type type = (World_Data::Tile_Type) Procedural_Generation::Random_Int(0, textureArraySize, seed);

          SDL_FRect rect;
          rect.x = i * size.width;
          rect.y = j * size.height;
          rect.w = size.width;
          rect.h = size.height;
          SDL_Texture *texture = World_Data::tiles[type];
//                if entities created do nothing
          if (tilesToRender[i][j].created) {

          }
//                else create them
          else {
//            Utilities::Log("creating tile objects");
            Generate_Trees(rect, tilesToRender[i][j]);
            tilesToRender[i][j].created = true;
          }

          SDL_FRect renderRect;
          renderRect.x = i * size.width - camera.screen.x;
          renderRect.y = j * size.height - camera.screen.y;
          renderRect.w = size.width;
          renderRect.h = size.height;
          SDL_RenderCopyF(Graphics::renderer, texture, NULL, &renderRect);
        }
      }
    }
  }
}