#pragma once
#include "entt/entt.hpp"
#include "graphics.h"
#include "procedural_generator.h"
#include "create_entities.h"

namespace  World_Data
{
    SDL_Texture* tileTextures[200];

    struct Tile_Objects {
        entt::entity objects[4];
    };

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

    SDL_Texture* tiles[25];

    enum Tile_Type {
        grass,
        sand,
        dirt
    };

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

namespace World
{
    World_Data::Offset worldOffset;
    World_Data::Tile_Size size;
    entt::registry zone;
    World_Data::Region region;

    void Init_Tiles() {
//        randomize
        worldOffset = { -100.0f, -100.0f };

        World_Data::tiles[0] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[1] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[2] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[3] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[4] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[5] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[6] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[7] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[8] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[9] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[10] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[11] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[12] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[13] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[14] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[15] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[16] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[17] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[18] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[19] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
        World_Data::tiles[20] = Graphics::createTexture("sprites/environment/ground_tiles/GroundTextures_Large_1/Grass_23_Tile.png");
    }

//    needs to run when the zone is in range
    void Generate_Region(World_Data::Offset &offset)
    {
        Init_Tiles();
        //go through all tiles and and ust the position to generate a random number, use that number to determine what objects are on each tile

//        world
        for (int w = offset.x; w < (offset.x + World_Data::width); w++)
        {
            for (int i = offset.y; i < (offset.x + World_Data::height); i++) {
                World_Data::Tile tile = region.board[w][i];
                //get tile position
                World_Data::Tile_Position position;

                position.x = w * size.width;
                position.y = i * size.height;

                //set tile data
                Procedural_Components::Seed seed;
                seed.seed = Procedural_Generation::Create_Initial_Seed(position.x, position.y);
//                tile.type = (World_Data::Tile_Type)Procedural_Generation::Random_Int(0, 1, seed);
            }
        }
    }

    void Generate()
    {
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

    void Get_Coords(int x, int &w)
    {
        x -= size.width;
        w = x / size.width;
    }

    void Get_Size()
    {

    }

    void Generate_Trees(entt::registry &zone, SDL_FRect rect)
    {
        Procedural_Components::Seed seed;
        seed.seed = Procedural_Generation::Create_Initial_Seed(rect.x, rect.y);
        int numMTrees = (World_Data::Tile_Type)Procedural_Generation::Random_Int(0, 5, seed);

//      cycle through trees create each one and check each successive one to make sure it isn't intersecting another object on the tile
        std::vector<entt::entity>trees;
        for (int i = 0; i < numMTrees; i++)
        {
            //create tree
//            position procedurally set within the tile
            float x, y;

//            not sure where to get these yet
            bool isRandom, isPlayer;

//            name is the key for SQLite db
//            class is in SQLite db
//            filepath is in SQLite db
            std::string name, entityClass, imgpath;

            Create_Entities::Create_Entity(zone, x, y, name, entityClass, isRandom, imgpath, isPlayer);
        }
    }

    void Render(Component::Camera &camera)
    {
        //generate objects only the tiles around the player

        //plug that in as the index ranges

        int x = 0;
        int y = 0;

        Get_Coords(camera.screen.x, x);
        Get_Coords(camera.screen.y, y);

        for (int i = x; i <= (x + ((camera.screen.w  + size.width + size.width) / size.width)); i++)
        {
            for (int j = y; j <= (y + ((camera.screen.h + size.height + size.height) / size.height)); j++)
            {
                // generate tile type
                Procedural_Components::Seed seed;
                seed.seed = Procedural_Generation::Create_Initial_Seed(i, j);
                World_Data::Tile_Type type = (World_Data::Tile_Type)Procedural_Generation::Random_Int(0, 19, seed);

                SDL_FRect renderRect;
                renderRect.x = i * size.width - camera.screen.x;
                renderRect.y = j * size.height - camera.screen.y;
                renderRect.w = size.width;
                renderRect.h = size.height;

                SDL_Rect clipRect = {0,0,(int)size.width,(int)size.height};

                SDL_Texture *texture = World_Data::tiles[(int)type];

                SDL_RenderCopyF(Graphics::renderer, texture, &clipRect, &renderRect);
            }
        }
    }
}
