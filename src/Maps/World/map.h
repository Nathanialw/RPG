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
        // TODO       randomize
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

                ///Create a random number of enemies
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

                //Create interactable objects
                if (World_Data::tilesEntities[0][i][j].tileTexture == 0) { //if the tile is a treasure room
                    int x = i;
                    int y = j;

                    SDL_FRect rect;
                    rect.x = i * World::size.width + (World::size.width / 2.0f);
                    rect.y = j * World::size.height + (World::size.width / 2.0f);

                    std::string objectName = Room::Populate_Objects(World::world[state].tileset, World_Data::tilesEntities[0][i][j].tileTexture, i, j);
                    std::cout << "Creating object: " << objectName << std::endl;
                    if (objectName.empty())
                        continue;

                    int xmlIndex = Game_Objects_Lists::tilesetObjectIndexes[World::world[state].tileset][objectName];

                    auto object_ID = Create_Entities::PVG_Building(zone, state, rect.x + x, rect.y + y, i, j, objectName, xmlIndex);
                    zone.emplace_or_replace<Component::Interactable>(object_ID);
                }
            }
        }
    }

    //spawn caves	 onto the map that never despawn
    void Init_Caves(entt::registry &zone, int &state, std::string &tileSet) {

        //cave
        if (state == 2) {
            std::cout << "Init_Caves: " << state << std::endl;

            Procedural_Components::Seed seed;
            int xmlIndex = 0;
            std::string objectName;

            int numTiles = World_Data::REGION_SIZE * World_Data::REGION_SIZE;
            int avgPerRegion = 1;

            for (int i = 0; i < World_Data::REGION_SIZE; ++i) {
                for (int j = 0; j < World_Data::REGION_SIZE; ++j) {
                    seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
                    int numObjects = Procedural_Generation::Random_Int(0, numTiles, seed);
                    if (numObjects < avgPerRegion) {
                        float x = Procedural_Generation::Random_float(0, (int) World::size.width, seed);
                        float y = Procedural_Generation::Random_float(0, (int) World::size.height, seed);

                        if (Game_Objects_Lists::tilesets[tileSet].empty())
                            return;

                        xmlIndex = Procedural_Generation::Random_Int(0, (int) Game_Objects_Lists::tilesets[tileSet].size(), seed);
                        objectName = Game_Objects_Lists::tilesets[tileSet][xmlIndex];

                        Create_Entities::PVG_Building(zone, state, (i * World::size.width) + x, (j * World::size.height) + y, i, j, objectName, xmlIndex);
                    }
                }
            }
            return;
        }

        //portal
        std::cout << "Init_Caves: " << state << " Portals" << std::endl;

        Procedural_Components::Seed seed;

        int numTiles = World_Data::REGION_SIZE * World_Data::REGION_SIZE;
        int avgPerRegion = 2;

        for (int i = 0; i < World_Data::REGION_SIZE; ++i) {
            for (int j = 0; j < World_Data::REGION_SIZE; ++j) {
                seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
                int numObjects = Procedural_Generation::Random_Int(0, numTiles, seed);
                if (numObjects < avgPerRegion) {
                    float x = Procedural_Generation::Random_float(0, (int) World::size.width, seed);
                    float y = Procedural_Generation::Random_float(0, (int) World::size.height, seed);

                    SDL_FRect rect;
                    rect.x = i * World::size.width + (World::size.width / 2.0f);
                    rect.y = j * World::size.height + (World::size.width / 2.0f);

                    std::string objectName = Room::Create_Portal(World::world[state].tileset, World_Data::tilesEntities[0][i][j].tileTexture, i, j);
                    std::cout << "Creating portal object: " << objectName << std::endl;
                    if (objectName.empty())
                        continue;

                    int xmlIndex = Game_Objects_Lists::tilesetObjectIndexes[World::world[state].tileset][objectName];

                    Create_Entities::PVG_Building(zone, state, rect.x + x, rect.y + y, i, j, objectName, xmlIndex);
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
            if (Game_Objects_Lists::tilesets[tileSet].empty())
                return;
            int xmlIndex = Procedural_Generation::Random_Int(0, (int) Game_Objects_Lists::tilesets[tileSet].size(), seed);
            std::string objectName = Game_Objects_Lists::tilesets[tileSet][xmlIndex];

            //will crash if it tries to insert objects onto the quad tree at the bottom edge of the map
            if (j < World_Data::REGION_SIZE - 1) {
                tile.objects[k].entity = (Create_Entities::PVG_Building(zone, state, rect.x + x, rect.y + y, i, j, objectName, xmlIndex));
            }
        }
    }

    void Generate_Labyrinth_Props(entt::registry &zone, int &state, SDL_FRect rect, std::string &tileSet, World_Data::Tile &tile) {
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

//		std::string objectName = Room::Populate_Props(tileSet, tile.tileTexture, i, j);
//		if (objectName.empty())
//			return;
//
//		int xmlIndex = Game_Objects_Lists::tilesetObjectIndexes[tileSet][objectName];
//
//		if (i == 0)
//			return;
//
//		//tile props
//		tile.objects[k].entity = (Create_Entities::PVG_Building(zone, state, rect.x + x, rect.y + y, i, j, objectName, xmlIndex));
//		zone.emplace_or_replace<Component::Interactable>(tile.objects[k].entity);
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
                        SDL_FRect rect;
                        rect.x = i * World::size.width;
                        rect.y = j * World::size.height;
                        rect.w = World::size.width;
                        rect.h = World::size.height;


                        if (!World_Data::tilesEntities[0][i][j].created) {
                            if (Texture_Data::Packer_Textures.contains(World::world[state].tileset)) {
                                Generate_Labyrinth_Props(zone, state, rect, World::world[state].tileset, World_Data::tilesEntities[0][i][j]);
                                World_Data::tilesEntities[0][i][j].tileObject = (Create_Entities::Create_Tile(zone, state, i, j, "labyrinth0_" + std::to_string(World_Data::tilesEntities[0][i][j].tileTexture)));
                                World_Data::tilesEntities[0][i][j].isTileObject = true;
                            } else {
                                Generate_Trees(zone, state, rect, World::world[state].tileset, World_Data::tilesEntities[0][i][j]);
                            }
                            World_Data::tilesEntities[0][i][j].created = true;
                        }

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
                    }
                }
            }
        }
    }
}// namespace Maps
