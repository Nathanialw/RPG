#pragma once
#include "db.h"

namespace Game_Objects_Lists {
  //tilesets
  std::unordered_map<std::string, std::vector<std::string>> tilesets;
  std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>> tilesetObjects;
  std::unordered_map<std::string, std::unordered_map<std::string, int>> indexes;
  std::unordered_map<std::string, int> objectIndexes;

  //tileset                                         //object        //index
  std::unordered_map<std::string, std::unordered_map<std::string, int>> tilesetObjectIndexes;

  //Units
  std::unordered_map<std::string, std::vector<db::Unit_Data>> units;


}// namespace Game_Objects_Lists
