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

    SDL_Texture* tiles[75];

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

    entt::basic_registry<entt::entity> zone;
    World_Data::Region region;

    void Init_Tiles() {
//        randomize
        worldOffset = { -100.0f, -100.0f };

        World_Data::tiles[0] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_1_Tile.png");
        World_Data::tiles[1] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_2_Tile.png");
        World_Data::tiles[2] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_3_Tile.png");
        World_Data::tiles[3] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_4_Tile.png");
        World_Data::tiles[4] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_5_Tile.png");
        World_Data::tiles[5] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_6_Tile.png");
        World_Data::tiles[6] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_7_Tile.png");
        World_Data::tiles[7] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_8_Tile.png");
        World_Data::tiles[8] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_9_Tile.png");
        World_Data::tiles[9] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Beach_10_Tile.png");
        World_Data::tiles[10] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_1_Tile.png");
        World_Data::tiles[11] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_2_Tile.png");
        World_Data::tiles[12] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_3_Tile.png");
        World_Data::tiles[13] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_4_Tile.png");
        World_Data::tiles[14] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_5_Tile.png");
        World_Data::tiles[15] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_6_Tile.png");
        World_Data::tiles[16] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_7_Tile.png");
        World_Data::tiles[17] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_8_Tile.png");
        World_Data::tiles[18] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_9_Tile.png");
        World_Data::tiles[19] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_10_Tile.png");
        World_Data::tiles[20] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_11_Tile.png");
        World_Data::tiles[21] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_12_Tile.png");
        World_Data::tiles[22] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_13_Tile.png");
        World_Data::tiles[23] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_14_Tile.png");
        World_Data::tiles[24] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_15_Tile.png");
        World_Data::tiles[25] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_16_Tile.png");
        World_Data::tiles[26] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_17_Tile.png");
        World_Data::tiles[27] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_18_Tile.png");
        World_Data::tiles[28] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_19_Tile.png");
        World_Data::tiles[29] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_20_Tile.png");
        World_Data::tiles[30] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_21_Tile.png");
        World_Data::tiles[31] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_22_Tile.png");
        World_Data::tiles[32] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_23_Tile.png");
        World_Data::tiles[33] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_24_Tile.png");
        World_Data::tiles[34] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_25_Tile.png");
        World_Data::tiles[35] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_26_Tile.png");
        World_Data::tiles[36] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_27_Tile.png");
        World_Data::tiles[37] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_28_Tile.png");
        World_Data::tiles[38] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_29_Tile.png");
        World_Data::tiles[39] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_30_Tile.png");
        World_Data::tiles[40] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_31_Tile.png");
        World_Data::tiles[41] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_32_Tile.png");
        World_Data::tiles[42] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_33_Tile.png");
        World_Data::tiles[43] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_34_Tile.png");
        World_Data::tiles[44] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_35_Tile.png");
        World_Data::tiles[45] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_36_Tile.png");
        World_Data::tiles[46] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_37_Tile.png");
        World_Data::tiles[47] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_38_Tile.png");
        World_Data::tiles[48] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_39_Tile.png");
        World_Data::tiles[49] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_40_Tile.png");
        World_Data::tiles[50] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_41_Tile.png");
        World_Data::tiles[51] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_42_Tile.png");
        World_Data::tiles[52] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_43_Tile.png");
        World_Data::tiles[53] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_44_Tile.png");
        World_Data::tiles[54] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_45_Tile.png");
        World_Data::tiles[55] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_46_Tile.png");
        World_Data::tiles[56] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_47_Tile.png");
        World_Data::tiles[57] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Dirt_48_Tile.png");
        World_Data::tiles[58] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_1_Tile.png");
        World_Data::tiles[59] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_2_Tile.png");
        World_Data::tiles[60] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_3_Tile.png");
        World_Data::tiles[61] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_4_Tile.png");
        World_Data::tiles[62] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_5_Tile.png");
        World_Data::tiles[63] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_6_Tile.png");
        World_Data::tiles[64] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_7_Tile.png");
        World_Data::tiles[65] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_8_Tile.png");
        World_Data::tiles[66] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_9_Tile.png");
        World_Data::tiles[67] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_10_Tile.png");
        World_Data::tiles[68] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_11_Tile.png");
        World_Data::tiles[69] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_12_Tile.png");
        World_Data::tiles[70] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_13_Tile.png");
        World_Data::tiles[71] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_14_Tile.png");
        World_Data::tiles[72] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_15_Tile.png");
        World_Data::tiles[73] = Graphics::createTexture("assets/sprites/environment/ground_tiles/GroundTextures_Large_1/Forest_16_Tile.png");
        World_Data::tiles[74] = Graphics::createTexture("assets/House_Blood.png");
    }

    void Create_Map (World_Data::Offset offset)
    {
        int h = 0;

        for (int w = offset.x; w < (offset.x + World_Data::width); w++)
        {
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
    void Generate_Region()
    {
        Init_Tiles();

        //go through all tiles and and ust the position to generate a random number, use that number to determine what objects are on each tile
//        Create_Map({0.0f, 0.0f});

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
                int textureArraySize = 74;
//                World_Data::Tile_Type type = (World_Data::Tile_Type)Procedural_Generation::Random_Int(0, textureArraySize, seed);

//                index to get ground texture
                int type = 73;

                SDL_FRect renderRect;
                renderRect.x = i * size.width - camera.screen.x;
                renderRect.y = j * size.height - camera.screen.y;
                renderRect.w = size.width;
                renderRect.h = size.height;

                SDL_Texture *texture = World_Data::tiles[type];

                SDL_RenderCopyF(Graphics::renderer, texture, NULL, &renderRect);
            }
        }
    }
}
