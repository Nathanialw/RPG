#pragma once
#include "labyrinth.h"


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
      int MAX_SIZE = Game_Objects_Lists::tilesetObjects[tileset]["chest"].size();
      int xmlIndex = Procedural_Generation::Random_Int(0, MAX_SIZE, seed);
      return Game_Objects_Lists::tilesetObjects[tileset]["chest"][xmlIndex];
      //      int xmlIndex = Procedural_Generation::Random_Int(0, 10, seed);
      //place a chest in the middle of the room
      //get the boundaries of the room
    } else
      return "";
  }


}// namespace Room