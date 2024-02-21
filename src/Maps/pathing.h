#include "dynamic_entity_loader.h"
#include "world.h"

// have a grid of the whole map
// set all cells that overlap with a collision box to impassable

//collision radii
//32
//64
//128
//256
//512
namespace Pathing {
  const int size32 = World::size.width * REGION_SIZE / 32;
  const int size64 = World::size.width * REGION_SIZE / 64;
  const int size128 = World::size.width * REGION_SIZE / 128;
  const int size256 = World::size.width * REGION_SIZE / 256;

  std::array<std::array<bool, size32>, size32> passableTerrain32;
  std::array<std::array<bool, size64>, size64> passableTerrain64;
  std::array<std::array<bool, size128>, size128> passableTerrain128;
  std::array<std::array<bool, size256>, size256> passableTerrain256;

  void asd(std::array<std::array<bool, size32>, size32> passableTerrain) {
    for (int i = 0; i < passableTerrain.size(); ++i) {
      for (int j = 0; j < passableTerrain[i].size(); ++j) {
        //check for a collision with any static objects to set map defaults

        //check for a collision with any dynamic objects every frame
        //      passableTerrain[i][j];
      }
    }
  }

}// namespace Pathing
 // maybe preset maps where the cell size is the size of the entity (ie. radius of 20 would use the grid with a cell size of 20x20)