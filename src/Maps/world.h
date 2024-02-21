#pragma once

namespace World {

  SDL_Texture *tileTextures[200];

  struct Tile_Objects {
    entt::entity objects[4];
  };

  //  std::array<std::array<bool, 64>, 64> Map;

  struct Tile_Size {
    const int width = 512;
    const int height = 512;
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
    dirt,
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

  struct Tileset {
    entt::registry zone;
    int previousZoneIndex;
    std::string music;
    std::string tilesetName = "forest_summer";
    std::string mobType;
    std::string tileset;
    std::string cave;
    Tile_Type type;
    std::vector<SDL_Texture *> tileTextures;
    bool loaded = false;
  };
  int numZones = 200;
  std::vector<Tileset> world(numZones);
  std::array<Tileset, 4> regions;
  //  std::array<Tileset, (int) Tile_Type::SIZE> tileSets;

  struct Current_Zone {
    int current;
    int next;
  };
  Current_Zone currentZone;

  int Zone_Count = 2;
  void increment_Zone() {
    Zone_Count++;
  }


  struct Loaded {
    int instance = 2;
  };

  //  Loaded loaded;
  struct TILE {
    int tile = 0;
    bool generated = false;
    SDL_FRect position = {};
  };

  TILE Game_Map[256][256];

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

  World::Offset worldOffset = {-100.0f, -100.0f};
  ;
  constexpr World::Tile_Size size;

  std::vector<entt::entity> Mouse_Hover_Entities;

  //  std::unordered_map<GameState, Tileset> tileSets;

  //    std::vector<Zone> World;
}// namespace World

namespace Map {

}// namespace Map