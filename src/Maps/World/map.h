#pragma once

#include "Objects/Collision/collision.h"
#include "SQLite_dialogue.h"
#include "create_entities.h"
#include "entt/entt.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/Map.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/TileLayer.hpp"
#include "world.h"
#include "world_update.h"
#include <iostream>
//#include "game_objects.h"
//#include "graphics.h"
#include "Maps/Procedural_Generation/number_generator.h"
//#include <map>
#include "Procedural_Generation//Labyrinth/treasure_room.h"

namespace Maps {

  void close_zone(entt::registry &scene) {
    scene.clear<>();
  }

  //  World::Region region;


  //  void Init_Tilesets() {
  //    std::string tilesetName[4] = {
  //        "forest_summer",
  //        "forest_winter",
  //        "forest_autumn",
  //        "beach"};
  //    for (int i = 0; i < World::regions.size(); ++i) {
  //      Entity_Loader::TileSet_Data data = Entity_Loader::Get_Tileset_Data(tilesetName[i]);
  //      World::regions[i].music = data.music;
  //      World::regions[i].mobType = data.tileUnits;
  //      World::regions[i].tileset = data.name;
  //      World::regions[i].tileTextures.emplace_back(Graphics::createTexture(data.tileset.c_str()));
  //    }
  //  }

  void Init_Tiles(int &instance, std::string &tilesetName) {
    //        randomize
    Entity_Loader::TileSet_Data data = Entity_Loader::Get_Tileset_Data(tilesetName);
    Utilities::Log(data.music);
    World::world[instance].music = data.music;
    Utilities::Log(World::world[instance].music);
    World::world[instance].mobType = data.tileUnits;
    World::world[instance].tileset = data.name;
    World::world[instance].tileTextures.emplace_back(Graphics::createTexture(data.tileset.c_str()));

    if (!data.tilesetXML.empty()) {
      Rendering_Components::Sheet_Data spritesheet;
      World::world[instance].sheetData = Texture_Packer::TexturePacker_Import_Tileset(spritesheet, data.name, data.name, data.tilesetXML.c_str());
    }

    World::world[instance].cave = "cave_entrances";
    //    World::tileSets[(int)World::Tile_Type::grass].music = "assets/music/nature.ogg";
  }

  //  void Create_Map(World::Offset offset, entt::registry &zone) {
  //    int h = 0;
  //
  //    for (int w = offset.x; w < (offset.x + World::width); w++) {
  //      for (int i = offset.y; i < (offset.x + World::height); i++) {
  //        //        World::Tile tile = region.board[w][i];
  //
  //        entt::entity entity = zone.create();
  //        //get tile position
  //        World::Tile_Position position;
  //
  //        position.x = w * World::size.width;
  //        position.y = i * World::size.height;
  //
  //        //set tile data
  //        Procedural_Components::Seed seed = zone.emplace_or_replace<Procedural_Components::Seed>(entity);
  //        seed.seed = Procedural_Generation::Create_Initial_Seed(position.x, position.y);
  //        h++;
  //      }
  //    }
  //    Utilities::Log("----MAP SIZE----");
  //    Utilities::Log(h);
  //  }

  //    needs to run when the zone is in range
  void Generate_Region(int &instance, std::string &tilesetName) {
    Init_Tiles(instance, tilesetName);
    //    Init_Tilesets();
    //go through all tiles and and ust the position to generate a random number, use that number to determine what objects are on each tile
    //        Create_Map({0.0f, 0.0f});
  }

  //  void Generate() {
  //generate objects only the tiles around the player

  //    //plug that in as the index ranges
  //    int w;
  //    int i;
  //
  //    int z;
  //    int j;
  //
  //    int a;
  //    int k;
  //
  //    //    World::Tile tile = region.board[w][i];
  ////    World::Tile_Position position = {};
  //    position.x = ((w * z) + a) * World::size.width;
  //    position.y = ((i * j) + k) * World::size.height;
  //    //                                auto entity = zone.create();
  //    //                                zone.emplace_or_replace<World::Tile>(entity, tile.x, tile.y);
  //    //                                    takes in tile coords as seed, outputs random number
  //
  //    Procedural_Components::Seed seed;
  //    seed.seed = Procedural_Generation::Create_Initial_Seed(position.x, position.y);
  //
  //    int treesExist = Procedural_Generation::Random_Int(0, 10, seed);
  //
  //    //        trees
  //        rocks
  //        plant objects

  //small monster camps
  //large monster camps
  //massive monster camps

  //        town
  //        tower

  //      house
  //        farm
  //  }

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
  //
  //  void Generate_Map() {
  //    int x = 0;
  //    int y = 0;
  //
  //    for (int i = 0; i <= 255; i++) {
  //      for (int j = 0; j <= 127; j++) {
  //        // generate tile type
  //        // Generate_Trees(SDL_FRect rect);
  //        Procedural_Components::Seed seed;
  //        seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
  //        int textureArraySize = 5;
  //
  //        World::TILE tile;
  //        SDL_FRect rect;
  //        rect.x = (float) ((i + x) * World::size.width);
  //        rect.y = (float) (j * World::size.height);
  //        rect.w = (float) World::size.width;
  //        rect.h = (float) World::size.height;
  //
  //        tile.position = rect;
  //        tile.generated = true;
  //        tile.tile = Procedural_Generation::Random_Int(0, textureArraySize, seed);
  //
  //        //        World::Game_Map[i][j] = tile;
  //        //Generate_Trees(tile.position);
  //      }
  //    }
  //  }

  //spawn dynamic units onto the map that never despawn
  void Init_Tile_Objects(entt::registry &zone, int &state, std::string &unitType) {
    auto &units = Game_Objects_Lists::units[unitType];
    if (units.empty())
      return;

    int numUnits = 0;
    Procedural_Components::Seed seed;

    for (int i = 0; i < World_Data::REGION_SIZE; ++i) {
      for (int j = 0; j < World_Data::REGION_SIZE; ++j) {
        seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
        int numObjects = Procedural_Generation::Random_Int(0, 100, seed);
        if (numObjects < 5) {
          float x = Procedural_Generation::Random_float(33, (int) World::size.width - 64, seed);
          float y = Procedural_Generation::Random_float(33, (int) World::size.height - 64, seed);

          int n = Procedural_Generation::Random_Int(0, units.size(), seed);
          db::Unit_Data data = Game_Objects_Lists::units[unitType][n];
          Social_Component::Summon summon;
          Component::Unit_Index unitIndex = {unitType, n};
          Create_Entities::Create_Entity(zone, state, (i * World::size.width) + x, (j * World::size.height) + y, data, false, summon, unitIndex);
          numUnits++;
        }
      }
    }
    std::cout << "number of units: " << numUnits << std::endl;
  }

  //spawn caves onto the map that never despawn
  void Init_Caves(entt::registry &zone, int &state, std::string &tileSet) {
    Procedural_Components::Seed seed;
    //    collisionBox;
    tmx::Vector2<float> offset = {0.0f, 0.0f};
    float x = 0.0f;
    float y = 0.0f;
    int xmlIndex = 0;
    std::string objectName;

    int numTiles = World_Data::REGION_SIZE * World_Data::REGION_SIZE;
    int avgPerRegion = 1;

    for (int i = 0; i < World_Data::REGION_SIZE; ++i) {
      for (int j = 0; j < World_Data::REGION_SIZE; ++j) {
        seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
        int numObjects = Procedural_Generation::Random_Int(0, numTiles, seed);
        if (numObjects < avgPerRegion) {
          x = Procedural_Generation::Random_float(0, (int) World::size.width, seed);
          y = Procedural_Generation::Random_float(0, (int) World::size.height, seed);

          if (Game_Objects_Lists::tilesets[tileSet].empty()) { return; }

          xmlIndex = Procedural_Generation::Random_Int(0, (int) Game_Objects_Lists::tilesets[tileSet].size(), seed);
          objectName = Game_Objects_Lists::tilesets[tileSet][xmlIndex];

          Create_Entities::PVG_Building(zone, state, (i * World::size.width) + x, (j * World::size.height) + y, i, j, objectName, xmlIndex);
        }
      }
    }
  }

  //The first time the tile is rendered generate objects but only the first time

  //spawn static units onto the map that despawn when you move too far away
  void Generate_Trees(entt::registry &zone, int &state, SDL_FRect rect, std::string &tileSet, World_Data::Tile &tile) {
    Procedural_Components::Seed seed;
    seed.seed = Procedural_Generation::Create_Initial_Seed(rect.x, rect.y);

    // the number of objects(trees and rocks) should be retrieved from an array created by simplex noise in order to create a proper forest
    // later, more trees = forest, more rocks = forest edge
    //    int numObjects = Procedural_Generation::Random_Int(0, 8, seed);
    int numObjects = World_Data::maxObjectsPerTile * tile.noise;

    for (int k = 0; k < numObjects; k++) {
      float x = Procedural_Generation::Random_float(0, (int) World::size.width, seed);
      float y = Procedural_Generation::Random_float(0, (int) World::size.height, seed);

      float i = rect.x / World::size.width;
      float j = rect.y / World::size.height;
      if (Game_Objects_Lists::tilesets[tileSet].empty()) { return; }
      int xmlIndex = Procedural_Generation::Random_Int(0, (int) Game_Objects_Lists::tilesets[tileSet].size(), seed);
      std::string objectName = Game_Objects_Lists::tilesets[tileSet][xmlIndex];

      //will crash if it tries to insert objects onto the quad tree at the bottom edge of the map
      if (j < World_Data::REGION_SIZE - 1) {
        tile.objects[k].entity = (Create_Entities::PVG_Building(zone, state, rect.x + x, rect.y + y, i, j, objectName, xmlIndex));
      }
    }
  }

  void Generate_Labyrinth_Objects(entt::registry &zone, int &state, SDL_FRect rect, std::string &tileSet, World_Data::Tile &tile) {
    //    Procedural_Components::Seed seed;
    //    seed.seed = Procedural_Generation::Create_Initial_Seed(rect.x, rect.y);

    // the number of objects(trees and rocks) should be retrieved from an array created by simplex noise in order to create a proper forest
    // later, more trees = forest, more rocks = forest edge
    //    int numObjects = Procedural_Generation::Random_Int(0, 8, seed);
    //    int numObjects = World_Data::maxObjectsPerTile;

    //calculate room type

    //query db to get object lists

    //calculate what kind of objects
    ///wall mounted ie. torches
    ///freestanding ie. chests, tables
    //set the object against the wall we need to know its radius to offset it from the wall

    //    for (int k = 0; k < numObjects; k++) {
    //
    //
    //          float x = Procedural_Generation::Random_float(33, (int) World::size.width - 65, seed);
    //          float y = Procedural_Generation::Random_float(33, (int) World::size.height - 65, seed);

    //      float i = rect.x / World::size.width;
    //      float j = rect.y / World::size.height;
    //      if (Game_Objects_Lists::tilesets[tileSet].empty()) { return; }
    //      int xmlIndex = Procedural_Generation::Random_Int(0, (int) Game_Objects_Lists::tilesets[tileSet].size(), seed);
    //    std::string objectName = Game_Objects_Lists::tilesets[tileSet][xmlIndex];

    int k = 0;
    float i = rect.x / World::size.width;
    float j = rect.y / World::size.height;
    float x = World::size.width / 2.0f;
    float y = World::size.height / 2.0f;

    std::string objectName = Room::Populate(tileSet, tile.tileTexture, i, j);
    if (objectName.empty()) return;

    int xmlIndex = Game_Objects_Lists::tilesetObjectIndexes[tileSet][objectName];


    if (i != 0) {
      tile.objects[k].entity = (Create_Entities::PVG_Building(zone, state, rect.x + x, rect.y + y, i, j, objectName, xmlIndex));
    }
  }


  //render tiles and instantiate the objects on the tiles
  void Render(entt::registry &zone, int &state, Component::Camera &camera) {
    int x = 0;
    int y = 0;

    Camera_Control::Get_Coords((int) camera.screen.x, x);
    Camera_Control::Get_Coords((int) camera.screen.y, y);

    for (int i = x; i < (x + ((camera.screen.w + World::size.width + World::size.width) / World::size.width)); i++) {
      for (int j = y; j < (y + ((camera.screen.h + World::size.height + World::size.height) / World::size.height)); j++) {
        if (i < 0 || j < 0) {
          continue;
        } else {
          if (i >= World_Data::REGION_SIZE || j >= World_Data::REGION_SIZE) {
            continue;
          } else {
            // generate tile type
            //            Procedural_Components::Seed seed;
            //            seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
            //            int textureArraySize = World::world[state].tileTextures.size();
            //            int tile = Procedural_Generation::Random_Int(0, textureArraySize, seed);

            SDL_FRect rect;
            rect.x = i * World::size.width;
            rect.y = j * World::size.height;
            rect.w = World::size.width;
            rect.h = World::size.height;


            if (!World_Data::tilesEntities[0][i][j].created) {
              if (Texture_Data::Packer_Textures.contains(World::world[state].tileset)) {
                Generate_Labyrinth_Objects(zone, state, rect, World::world[state].tileset, World_Data::tilesEntities[0][i][j]);
                World_Data::tilesEntities[0][i][j].tileObject = (Create_Entities::Create_Tile(zone, state, i, j, "labyrinth0_" + std::to_string(World_Data::tilesEntities[0][i][j].tileTexture)));
                World_Data::tilesEntities[0][i][j].isTileObject = true;
              } else {
                Generate_Trees(zone, state, rect, World::world[state].tileset, World_Data::tilesEntities[0][i][j]);
              }
              World_Data::tilesEntities[0][i][j].created = true;
            }

            // fog needs to be rendered over the tile

            //an area around the player will trigger the fog to not render

            SDL_FRect renderRect;
            renderRect.x = (float) i * World::size.width - camera.screen.x;
            renderRect.y = (float) j * World::size.height - camera.screen.y;
            renderRect.w = (float) World::size.width;
            renderRect.h = (float) World::size.height;

            SDL_Texture *texture = World::world[state].tileTextures[0];

            if (Texture_Data::Packer_Textures.contains(World::world[state].tileset)) {
              auto clipRect = Texture_Data::Packer_Textures[World::world[state].tileset].frameList[Texture_Data::Packer_Textures[World::world[state].tileset].frameList.size() - 1].clip;
              SDL_RenderCopyF(Graphics::renderer, texture, &clipRect, &renderRect);
              clipRect = Texture_Data::Packer_Textures[World::world[state].tileset].frameList[World_Data::tilesEntities[0][i][j].tileTexture].clip;
              SDL_RenderCopyF(Graphics::renderer, texture, &clipRect, &renderRect);
            } else {
              SDL_RenderCopyF(Graphics::renderer, texture, nullptr, &renderRect);
            }

            //            SDL_RenderCopyF(Graphics::renderer, Graphics::fog, nullptr, &renderRect);
          }
        }
      }
    }
  }


  tmx::Map map;
  SDL_Texture *pTexture = nullptr;
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
