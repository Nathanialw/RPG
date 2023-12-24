#pragma once

namespace World {
  
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

  enum class GameState {
    overworld,
    cave,
    MODES,
    restart,
    exit
  };
  GameState gamestate = GameState::overworld;

  enum class Tile_Type {
    grass,
    sand,
    dirt,
    beach,
    desert,
    ice,
    mud,
    rock,
    scorched,
    snow,
    swamp,
    volcanic,
    SIZE
  };
  std::array<std::vector<SDL_Texture*>, (int)Tile_Type::SIZE> tileSets;

  std::unordered_map<GameState, Tile_Type> tileType = {
      {GameState::overworld, Tile_Type::grass },
      {GameState::cave, Tile_Type::volcanic }
  };

  struct TILE {
    int tile;
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
  
  World::Offset worldOffset;
  World::Tile_Size size;
  



  std::vector<entt::entity> Mouse_Hover_Entities;
  entt::basic_registry<entt::entity> zone;
//  std::vector<entt::entity> Mouse_Hover_EntitiesOverworld;

  entt::basic_registry<entt::entity> cave;
//  std::vector<entt::entity> Mouse_Hover_EntitiesCave;

  //    std::vector<Zone> World;
}// namespace World

namespace Map {

}// namespace World