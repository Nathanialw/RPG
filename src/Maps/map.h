#pragma once

#include "SQLite_dialogue.h"
#include "collision.h"
#include "create_entities.h"
#include "dynamic_entity_loader.h"
#include "entt/entt.hpp"
#include "world.h"
#include <iostream>
#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/TileLayer.hpp>
//#include "game_objects.h"
//#include "graphics.h"
#include "procedural_generator.h"
//#include <map>

namespace Maps {

  void close_zone(entt::registry &scene) {
    scene.clear<>();
  }

  World::Region region;


  void Init_Tiles(int &instance, std::string &tilesetName) {
    //        randomize
    Entity_Loader::TileSet_Data data = Entity_Loader::Get_Tileset_Data(tilesetName);
    Utilities::Log(data.music);
    World::world[instance].music = data.music.c_str();
    Utilities::Log(World::world[instance].music);
    World::world[instance].mobType = data.tileUnits;
    World::world[instance].tileset = data.name;
    World::world[instance].tileTextures.emplace_back(Graphics::createTexture(data.tileset.c_str()));
    //    World::tileSets[(int)World::Tile_Type::grass].music = "assets/music/nature.ogg";
  }

  void Create_Map(World::Offset offset, entt::registry &zone) {
    int h = 0;

    for (int w = offset.x; w < (offset.x + World::width); w++) {
      for (int i = offset.y; i < (offset.x + World::height); i++) {
        World::Tile tile = region.board[w][i];

        entt::entity entity = zone.create();
        //get tile position
        World::Tile_Position position;

        position.x = w * World::size.width;
        position.y = i * World::size.height;

        //set tile data
        Procedural_Components::Seed seed = zone.emplace_or_replace<Procedural_Components::Seed>(entity);
        seed.seed = Procedural_Generation::Create_Initial_Seed(position.x, position.y);
        h++;
      }
    }
    Utilities::Log("----MAP SIZE----");
    Utilities::Log(h);
  }

  //    needs to run when the zone is in range
  void Generate_Region(int &instance, std::string &tilesetName) {
    Init_Tiles(instance, tilesetName);

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

    World::Tile tile = region.board[w][i];
    World::Tile_Position position;
    position.x = ((w * z) + a) * World::size.width;
    position.y = ((i * j) + k) * World::size.height;
    //                                auto entity = zone.create();
    //                                zone.emplace_or_replace<World::Tile>(entity, tile.x, tile.y);
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
    x -= World::size.width;
    w = x / World::size.width;
  }

  void Get_Size() {
  }

  std::vector<bool> tiles = {};
  //
  //  void Renders(Component::Camera &camera) {
  //    //generate objects only the tiles around the player
  //    //plug that in as the index ranges
  //    int x = 0;
  //    int y = 0;
  //    Get_Coords(camera.screen.x, x);
  //    Get_Coords(camera.screen.y, y);
  //
  //    for (int i = x; i <= (x + ((camera.screen.w + World::size.width + World::size.width) / World::size.width)); i++) {
  //      for (int j = y; j <= (y + ((camera.screen.h + World::size.height + World::size.height) / World::size.height)); j++) {
  //        if (i < 0 || j < 0 || j > 127 || i > 255) {
  //          continue;
  //        }
  //        SDL_FRect tile = World::Game_Map[i][j].position;
  //
  //        SDL_FRect renderRect;
  //        renderRect.x = tile.x - camera.screen.x;
  //        renderRect.y = tile.y - camera.screen.y;
  //        renderRect.w = World::size.width;
  //        renderRect.h = World::size.height;
  //
  //        SDL_Texture *texture = World::tileSets[(int)World::Tile_Type::grass][World::Game_Map[i][j].tile];
  //        SDL_RenderCopyF(Graphics::renderer, texture, NULL, &renderRect);
  //      }
  //    }
  //  }

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

        World::TILE tile;
        SDL_FRect rect;
        rect.x = ((i + x) * World::size.width);
        rect.y = (j * World::size.height);
        rect.w = World::size.width;
        rect.h = World::size.height;

        tile.position = rect;
        tile.generated = true;
        tile.tile = Procedural_Generation::Random_Int(0, textureArraySize, seed);

        World::Game_Map[i][j] = tile;
        //Generate_Trees(tile.position);
      }
    }
  }

  void Init_Tile_Objects(entt::registry &zone, int &state, std::string &unitType) {
    int numUnits = 0;
    float x = 0.0f;
    float y = 0.0f;
    Procedural_Components::Seed seed;

    for (int i = 0; i < 255; ++i) {
      for (int j = 0; j < 127; ++j) {
        seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
        int numObjects = Procedural_Generation::Random_Int(0, 100, seed);
        if (numObjects < 5) {
          x = Procedural_Generation::Random_float(0, (int) World::size.width, seed);
          y = Procedural_Generation::Random_float(0, (int) World::size.height, seed);

          int n = Procedural_Generation::Random_Int(0, Game_Objects_Lists::units[unitType].size(), seed);
          db::Unit_Data data = Game_Objects_Lists::units[unitType][n];
          Create_Entities::Create_Entity(zone, state, (i * World::size.width) + x, (j * World::size.height) + y, "", false, data, false);
          numUnits++;
        }
      }
    }
    std::cout << "number of units: " << numUnits << std::endl;
  }

  void Init_Caves(entt::registry &zone, int &state, std::string &tileSet) {
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


    for (int i = 0; i < 255; ++i) {
      for (int j = 0; j < 127; ++j) {
        seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
        int numObjects = Procedural_Generation::Random_Int(0, 100, seed);
        if (numObjects < 5) {
          x = Procedural_Generation::Random_float(0, (int) World::size.width, seed);
          y = Procedural_Generation::Random_float(0, (int) World::size.height, seed);

          if (Game_Objects_Lists::tilesets[tileSet].size() <= 0) { return; }

          xmlIndex = Procedural_Generation::Random_Int(0, Game_Objects_Lists::tilesets[tileSet].size(), seed);
          objectName = Game_Objects_Lists::tilesets[tileSet][xmlIndex];

          if (objectName == "Rock_3_1" ||
              objectName == "Rock_3_2" ||
              objectName == "Rock_3_3" ||
              objectName == "Rock_3_4" ||
              objectName == "Rock_3_5" ||
              objectName == "Rock_3_6" ||
              objectName == "Rock_3_7" ||
              objectName == "Rock_3_8" ||
              objectName == "Rock_2_1" ||
              objectName == "Rock_2_2" ||
              objectName == "Rock_2_3" ||
              objectName == "Rock_2_4" ||
              objectName == "Rock_2_5" ||
              objectName == "Rock_2_6" ||
              objectName == "Rock_2_7" ||
              objectName == "Rock_2_8") {
            Create_Entities::PVG_Building(zone, state, (i * World::size.width) + x, (j * World::size.height) + y, i, j, objectName, xmlIndex, aabb, pointVecs, line);
          }
        }
      }
    }
  }

  //    The first time the tile is rendered generate objects but only the first time
  int ii = 0;

  //position of tile
  void Generate_Trees(entt::registry &zone, int &state, SDL_FRect rect, std::string &tileSet, Tile &tile) {
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
    int numObjects = Procedural_Generation::Random_Int(1, 5, seed);

    for (int k = 0; k < numObjects; k++) {
      x = Procedural_Generation::Random_float(0, (int) World::size.width, seed);
      y = Procedural_Generation::Random_float(0, (int) World::size.height, seed);

      float i = rect.x / World::size.width;
      float j = rect.y / World::size.height;
      if (Game_Objects_Lists::tilesets[tileSet].size() <= 0) { return; }
      xmlIndex = Procedural_Generation::Random_Int(0, Game_Objects_Lists::tilesets[tileSet].size(), seed);
      objectName = Game_Objects_Lists::tilesets[tileSet][xmlIndex];
      if (objectName == "Rock_3_1" ||
          objectName == "Rock_3_2" ||
          objectName == "Rock_3_3" ||
          objectName == "Rock_3_4" ||
          objectName == "Rock_3_5" ||
          objectName == "Rock_3_6" ||
          objectName == "Rock_3_7" ||
          objectName == "Rock_3_8" ||
          objectName == "Rock_2_1" ||
          objectName == "Rock_2_2" ||
          objectName == "Rock_2_3" ||
          objectName == "Rock_2_4" ||
          objectName == "Rock_2_5" ||
          objectName == "Rock_2_6" ||
          objectName == "Rock_2_7" ||
          objectName == "Rock_2_8") {
        continue;
      }
      if (i != 0) {
        tile.objects.emplace_back(Create_Entities::PVG_Building(zone, state, rect.x + x, rect.y + y, i, j, objectName, xmlIndex, aabb, pointVecs, line));
      }
    }
  }

  void Render(entt::registry &zone, int &state, Component::Camera &camera) {
    int x = 0;
    int y = 0;

    Get_Coords(camera.screen.x, x);
    Get_Coords(camera.screen.y, y);

    for (int i = x; i <= (x + ((camera.screen.w + World::size.width + World::size.width) / World::size.width)); i++) {
      for (int j = y; j <= (y + ((camera.screen.h + World::size.height + World::size.height) / World::size.height)); j++) {
//        if (i == 0) {
          // generate tile type
          Procedural_Components::Seed seed;
          seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
          //          int textureArraySize = World::tileSets[(int)World::tileType[state]].tileTextures.size();
          int textureArraySize = World::world[state].tileTextures.size();
          int tile = Procedural_Generation::Random_Int(0, textureArraySize, seed);
          SDL_FRect rect;
          rect.x = i * World::size.width;
          rect.y = j * World::size.height;
          rect.w = World::size.width;
          rect.h = World::size.height;
          //          SDL_Texture *texture = World::tileSets[(int)World::tileType[state]].tileTextures[tile];
          SDL_Texture *texture = World::world[state].tileTextures[tile];
          //                if entities created do nothing
          if (tilesEntities[i][j].created) {

          }
          //                else create them
          else {
            //            Utilities::Log("creating tile objects");
            Generate_Trees(zone, state, rect, World::world[state].tileset, tilesEntities[i][j]);
            tilesEntities[i][j].created = true;
          }

          SDL_FRect renderRect;
          renderRect.x = i * World::size.width - camera.screen.x;
          renderRect.y = j * World::size.height - camera.screen.y;
          renderRect.w = World::size.width;
          renderRect.h = World::size.height;
          SDL_RenderCopyF(Graphics::renderer, texture, NULL, &renderRect);
//        }
      }
    }
  }

  tmx::Map map;
  SDL_Texture *pTexture = NULL;
  //
  //  void Create_Map(entt::registry &zone, int &state) {
  //    Generate_Region();
  //
  //    if (map.load("assets/maps/aaaa.tmx")) {
  //      for (auto &spriteSheet: map.getTilesets()) {
  //        std::string name = spriteSheet.getName();
  //      }
  //
  //      std::cout << "Loaded Map version: " << map.getVersion().upper << ", " << map.getVersion().lower << std::endl;
  //
  //      const auto &layers = map.getLayers();
  //      std::cout << "Map has " << layers.size() << " layers" << std::endl;
  //
  //      for (const auto &layer: layers) {
  //        if (layer->getName() == "ground") {
  //          if (layer->getType() == tmx::Layer::Type::Tile) {
  //            for (auto &tiles: map.getTilesets()) {
  //              std::string name = tiles.getName();
  //              std::string imgpath = tiles.getImagePath();
  //              Graphics::Create_Tileset(name, imgpath.c_str());
  //            }
  //          }
  //        } else if (layer->getName() == "widgets") {
  //          if (layer->getType() == tmx::Layer::Type::Tile) {
  //            for (auto &tiles: map.getTilesets()) {
  //              std::string name = tiles.getName();
  //              std::string imgpath = tiles.getImagePath();
  //              Graphics::Create_Tileset(name, imgpath.c_str());
  //            }
  //          }
  //        } else if (layer->getName() == "taller_widgets1") {
  //          if (layer->getType() == tmx::Layer::Type::Tile) {
  //            for (auto &tiles: map.getTilesets()) {
  //              std::string name = tiles.getName();
  //              std::string imgpath = tiles.getImagePath();
  //              Graphics::Create_Tileset(name, imgpath.c_str());
  //            }
  //          }
  //        } else if (layer->getName() == "unit_spawns") {
  //          for (auto &object: layer->getLayerAs<tmx::ObjectGroup>().getObjects()) {
  //            auto &position = object.getPosition();
  //            std::string tilePath = object.getTilesetName();
  //            std::string entity_class = object.getClass();
  //
  //            // translates isometric position to world position
  ////	    float tileWidth = 128;
  ////	    float tileHeight = 64;
  ////	    float numTilesX = position.x / tileWidth;
  ////	    float numTilesY = position.y / tileHeight;
  ////	    float x = 64.0f + position.x - (numTilesY * tileWidth / 2.0f);
  ////	    float y = (numTilesX * tileHeight) + (position.y / 2.0f);
  //
  //            float tileWidth = 512;
  //            float tileHeight = 512;
  //            float numTilesX = position.x / tileWidth;
  //            float numTilesY = position.y / tileHeight;
  //            float x = position.x;
  //            float y = position.y;
  //
  //            //if it is random it needs to grab a name from a unit that was already loaded into graphics or default to a default unit name
  //            //get an array of all the potential names, check each on against teh std::map of graphics, keep all the ones already there and pick a random one
  //
  //            // gets the tilset vector index and the image index within the vector
  //            auto id = object.getTileID();
  //
  //            auto &tilesets = Maps::map.getTilesets();
  //            int p = Maps::map.getTilesets().size() - 1;
  //
  //            int tilesetCount = p;
  //            for (tilesetCount = p; tilesetCount >= 0; --tilesetCount) {
  //              const tmx::Tileset *tileset = &tilesets[tilesetCount];
  //              if (tileset->getFirstGID() - 1 <= id) {
  //                id -= tilesets[tilesetCount].getFirstGID() - 1;
  //                std::string name = tileset->getName();
  //                break;
  //              }
  //            }
  //            id -= 1;
  //
  //            auto klk = map.getTilesets().at(tilesetCount);
  //            auto ggg = klk.getTiles();
  //            auto aaaa = ggg.at(id);
  //            auto imgPath = aaaa.imagePath;
  //
  //            //gets the collision box/boxes for objects
  //            std::vector<tmx::Object> collision_boxes;
  //            Collision::aabb aabb;
  //            tmx::Vector2<float> offset = {0.0f, 0.0f};
  //            std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
  //            Component::Line_Segment line;
  //
  //            std::string tilesetName = object.getTilesetName();
  //            auto &templateTilesets = map.getTemplateTilesets();
  //
  //            std::string templateName = "";
  //            if (tilesetName != "") {
  //              auto ffs = templateTilesets.at(tilesetName);
  //              std::string templateName = ffs.getName();
  //              u_int32_t tilesetIndex = 0;
  //              auto fas = ffs.getTiles();
  //
  //              if (fas.size() > 1) {
  //                auto asa = fas.at(object.getTileID() - 1);
  //                tilesetIndex = fas.at(object.getTileID() - 1).ID;
  //                std::string templateFile = asa.imagePath.substr(asa.imagePath.find_last_of("/\\") + 1);
  //                std::string::size_type const p(templateFile.find_last_of('.'));
  //                templateName = templateFile.substr(0, p);
  //              }
  //
  //              if (ffs.getTiles().size() > tilesetIndex) {
  //                auto s = ffs.getTiles().at(tilesetIndex);
  //
  //                collision_boxes = s.objectGroup.getObjects();
  //                float sizeX = s.imageSize.x;
  //                float sizeY = s.imageSize.y;
  //
  //                int j = 0;
  //                for (auto rects: collision_boxes) {
  //                  aabb.hx = rects.getAABB().width / 2.0f;
  //                  aabb.hy = rects.getAABB().height / 2.0f;
  //                  float aabbx = rects.getAABB().left;
  //                  float aabby = rects.getAABB().top;
  //
  //                  if (rects.getShape() == tmx::Object::Shape::Polygon) {
  //
  //                  }
  //
  //                  if (rects.getShape() == tmx::Object::Shape::Point) {
  //                    line.p[j].x = x - ((sizeX / 2.0f) - aabbx);
  //                    line.p[j].y = y - (sizeY - aabby);
  //                    j++;
  //                  } else if (rects.getPoints().size() == 1) {
  //                    std::vector<tmx::Vector2<float>> pointVec = rects.getPoints();
  //                    for (int i = 0; i < pointVec.size(); i++) {
  //                      offset = pointVec[i];
  //                    }
  //                  } else if (rects.getPoints().size() > 1) {
  //                    std::vector<tmx::Vector2<float>> pointVec = rects.getPoints();
  //                    for (int i = 0; i < pointVec.size(); i++) {
  //                      pointVec[i].x = pointVec[i].x - ((sizeX / 2.0f) - aabbx);
  //                      pointVec[i].y = pointVec[i].y - (sizeY - aabby);
  //                    }
  //                    pointVecs.emplace_back(pointVec);
  //                  }
  //                }
  //
  //                bool player = false;
  //                bool is_random = false;
  //
  //                for (auto property: object.getProperties()) {
  //                  if (property.getName() == "player") {
  //                    player = property.getBoolValue();
  //                  }
  //                  if (property.getName() == "is_random") {
  //                    is_random = property.getBoolValue();
  //                  }
  //                }
  //
  //                std::string texture = templateTilesets.at(tilesetName).getImagePath();
  //                std::string temptexture = "assets/" + texture;
  //                db::Unit_Data data = {templateName, imgPath, "", ""};
  //
  //                // Utilities::Log(texture);
  //
  //                if (!Create_Entities::PVG_Building(zone, state, x, y, x, y, templateName, tilesetIndex, aabb, pointVecs, line)) {
  //                  if (!Create_Entities::Polygon_Building(zone, state, x, y, templateName, entity_class, texture, aabb, pointVecs, line, offset)) {
  //                    Create_Entities::Create_Entity(zone, state, x, y, entity_class, is_random, data, player);
  //                  }
  //                }
  //              }
  //            } else {
  //              std::string templateFile = imgPath.substr(imgPath.find_last_of("/\\") + 1);
  //              std::string::size_type const p(templateFile.find_last_of('.'));
  //              templateName = templateFile.substr(0, p);
  //
  //              auto s = aaaa;
  //
  //              collision_boxes = s.objectGroup.getObjects();
  //              float sizeX = s.imageSize.x;
  //              float sizeY = s.imageSize.y;
  //
  //              int j = 0;
  //              for (auto rects: collision_boxes) {
  //                aabb.hx = rects.getAABB().width / 2.0f;
  //                aabb.hy = rects.getAABB().height / 2.0f;
  //                float aabbx = rects.getAABB().left;
  //                float aabby = rects.getAABB().top;
  //
  //                if (rects.getShape() == tmx::Object::Shape::Point) {
  //                  offset = rects.getPosition();
  //                }
  //
  //                if (rects.getShape() == tmx::Object::Shape::Point && rects.getPoints().size() > 1) {
  //                  line.p[j].x = x - ((sizeX / 2.0f) - aabbx);
  //                  line.p[j].y = y - (sizeY - aabby);
  //                  j++;
  //                } else if (rects.getPoints().size() > 0) {
  //                  std::vector<tmx::Vector2<float>> pointVec = rects.getPoints();
  //                  for (int i = 0; i < pointVec.size(); i++) {
  //                    pointVec[i].x = pointVec[i].x - ((sizeX / 2.0f) - aabbx);
  //                    pointVec[i].y = pointVec[i].y - (sizeY - aabby);
  //                  }
  //                  pointVecs.emplace_back(pointVec);
  //                }
  //              }
  //              Utilities::Log(templateName);
  //              bool player = false;
  //              bool is_random = false;
  //              db::Unit_Data data = {templateName, imgPath, "", ""};
  //
  //              // imgPath is the location of the image used in Tiled, not useful for collections of images, the correct image path is derived later
  //              if (!Create_Entities::PVG_Building(zone, state, x, y, x, y, templateName, id, aabb, pointVecs, line)) {
  //                if (!Create_Entities::Polygon_Building(zone, state, x, y, templateName, entity_class, imgPath, aabb, pointVecs, line, offset)) {
  //                  Create_Entities::Create_Entity(zone, state, x, y, entity_class, is_random, data, player);
  //                }
  //              }
  //            }
  //          }
  //        } else {
  //          std::cout << "Failed loading map" << std::endl;
  //        }
  //      }
  //    }
  //  }
}// namespace Maps
