#pragma once
#include "SDL2/SDL.h"
#include "XML_Parsers/texture_data.h"
#include "entt/entt.hpp"
#include "string"

namespace World {

  struct Tile_Size {
    const int width = 512;
    const int height = 512;
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
    std::unordered_map<std::string, Rendering_Components::Sheet_Data> *sheetData;
    bool loaded = false;
  };

  std::vector<Tileset> world(World_Data::numZones);
  //  std::array<Tileset, 4> regions;

  struct Current_Zone {
    int current;
    int next;
  };
  Current_Zone currentZone;

  int Zone_Count = 2;
  void increment_Zone() {
    Zone_Count++;
  }

  constexpr Tile_Size size;

  std::vector<entt::entity> Mouse_Hover_Entities;


}// namespace World
