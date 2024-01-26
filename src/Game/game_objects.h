#pragma once
#include "db.h"
#include "map"

namespace Game_Objects_Lists {
//tilesets
  std::unordered_map<std::string, std::vector<std::string>> tilesets;
  std::unordered_map<std::string, std::unordered_map<std::string, int>> indexes;

  //Units
  std::unordered_map<std::string, std::vector<db::Unit_Data>> units;



  void Get_tileset(std::string &imagePath, std::vector<std::string> &tilesetVec) {

    if (imagePath == "forest_objects.png") {
//      forestTreeVec = tilesetVec;
    }
//    else if (imagePath == "forest_background.png") {
//      forestTreeVec = tilesetVec;
//    }
//    else if (imagePath == "pool1.png") {
//      bloodPoolVec = tilesetVec;
//    }
//    else if (imagePath == "splatter1.png") {
//      bloodSplatterVec = tilesetVec;
//    }
    else {
      std::cout << "vector imagePath not found: " << imagePath << std::endl;
    }
  }

}
