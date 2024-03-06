#pragma once
#include "labyrinth.h"
#include <iterator>

namespace Room {

  /* Returns an array of strings of types entities in the room
   * ie. chests, tombs etc
   */
  std::string Populate(const std::string &tileset, const int &tileType, const int &x, const int &y) {
    Procedural_Components::Seed seed;
    seed.seed = Procedural_Generation::Create_Initial_Seed(x, y);

    ///(lootable) contains good loot coffer
    //chest casket tomb

    ///has a corpse on it (or someone to save)
    //altar

    ///(lootable) contains bad loot crate
    //barrel bag

    ///(lootable) contains reagents
    //mining node boneStack ebonSkulls

    ///environment objects
    //stalagmite tortureDevice

    ///small objects can spawn on it chairs spawn next to it
    // table stool

    ///brazier
    //light
    if (tileType == 0) {

      //      int MAX_TYPES = Game_Objects_Lists::tilesetObjects[tileset].size();
      //      int randomType = Procedural_Generation::Random_Int(0, MAX_TYPES, seed);
      //
      //      int MAX_OBJECTS = Game_Objects_Lists::tilesetObjects[tileset][randomType].size();
      //      int randomObject = Procedural_Generation::Random_Int(0, MAX_OBJECTS, seed);


      //      return Game_Objects_Lists::tilesetObjects[tileset]["chest"][randomObject][direction];
      return Game_Objects_Lists::tilesetObjects[tileset]["chest"]["Chest1"][0];
      //      int xmlIndex = Procedural_Generation::Random_Int(0, 10, seed);
      //place a chest in the middle of the room
      //get the boundaries of the room
    } else
      return "";
  }


}// namespace Room