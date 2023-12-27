#pragma once

#include <array>
#include <unordered_map>
#include <vector>
#include <string>
#include <map>

#include "SQLite_unit_data.h"
#include "tinyxml/tinyxml2.h"
#include "components.h"
#include "utilities.h"
#include "social_control.h"
#include "game_objects.h"

namespace Load_Object_List {

  std::array<std::vector<std::string>[(int) Social_Component::Race::SIZE][(int) Component::Foliage::SIZE], (int) Component::Entity_Type::SIZE> gameObjects;

  struct Game_Objects {
    std::vector<std::vector<std::string>[(int) Social_Component::Race::SIZE][(int) Component::Entity_Type::SIZE][(int) Component::Foliage::SIZE]> gameObjects;
    std::vector<std::string> spells[(int) Social_Component::Race::SIZE][(int) Component::Entity_Type::SIZE][(int) Component::Spells::SIZE];
    std::vector<std::string> items[(int) Social_Component::Race::SIZE][(int) Component::Entity_Type::SIZE][(int) Component::Items::SIZE];
    std::vector<std::string> objects[(int) Social_Component::Race::SIZE][(int) Component::Entity_Type::SIZE][(int) Component::Objects::SIZE];
    std::vector<std::string> props[(int) Social_Component::Race::SIZE][(int) Component::Entity_Type::SIZE][(int) Component::Props::SIZE];
    std::vector<std::string> buildings[(int) Social_Component::Race::SIZE][(int) Component::Entity_Type::SIZE][(int) Component::Buildings::SIZE];

    //    std::unordered_map<std::string, std::vector<std::string>> gameObjects; 
  };

  Component::Entity_Type Get_Type(std::string &typeStr) {

    if (typeStr == "unit") {
      return Component::Entity_Type::unit;
    } else if (typeStr == "foliage") {
      return Component::Entity_Type::foliage;
    } else if (typeStr == "spell") {
      return Component::Entity_Type::spell;
    } else if (typeStr == "item") {
      return Component::Entity_Type::item;
    } else if (typeStr == "object") {
      return Component::Entity_Type::object;
    } else if (typeStr == "prop") {
      return Component::Entity_Type::prop;
    } else if (typeStr == "building") {
      return Component::Entity_Type::building;
    }
    std::cout << "Type: " << typeStr << " from db not found; aborting. " << std::endl;
    return Component::Entity_Type::SIZE;
  }

  int Get_Subtype(std::string &type) {


    return 1;
  };

  std::vector<std::string> Get_Entities(std::string &typeStr) {
    if (typeStr == "unit") {
      Utilities::Log("units");
      return Entity_Loader::Get_All_Units_Of_Type(typeStr);
    } else if (typeStr == "foliage") {
      Utilities::Log("foliage");
      return Entity_Loader::Get_All_Units_Of_Type(typeStr);;
    } else if (typeStr == "building") {
      Utilities::Log("building");
      return Entity_Loader::Get_All_Objects_Of_Type(typeStr);;
    } else if (typeStr == "spell") {
      Utilities::Log("spells");
      return Entity_Loader::Get_All_Units_Of_Type(typeStr);
    } else if (typeStr == "item") {
      Utilities::Log("items");
      return Entity_Loader::Get_All_Items_Of_Type(typeStr);
    } else if (typeStr == "object") {
      Utilities::Log("objects");
      return Entity_Loader::Get_All_Objects_Of_Type(typeStr);
    } else if (typeStr == "prop") {
      Utilities::Log("props");
      return Entity_Loader::Get_All_Objects_Of_Type(typeStr);
    }
    std::cout << "Type: " << typeStr << " from db not found; aborting. " << std::endl;
    std::vector<std::string> empty;
    return empty;
  }

  using namespace Social_Component;
  using namespace Component;

  std::vector<std::string> Get_Buildings(Social_Component::Race race, Component::Entity_Type type, Component::Buildings buildingType) {
    return gameObjects[(int) type][(int) race][(int) buildingType];
  };

  std::vector<std::string> Get_Props(Social_Component::Race race, Component::Entity_Type type, Component::Props propType) {
    return gameObjects[(int) type][(int) race][(int) propType];
  };

  std::vector<std::string> Get_Units(Social_Component::Race race, Component::Entity_Type type, Component::Units unitType) {
    return gameObjects[(int) type][(int) race][(int) unitType];
  };

  std::vector<std::string> Load_Tileset(const char * xmlPath) {
    tinyxml2::XMLDocument spriteSheetData;
    spriteSheetData.LoadFile(xmlPath);
    tinyxml2::XMLElement *pSpriteElement;
    pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");

    std::string imgPath = spriteSheetData.RootElement()->Attribute("imagePath");

    std::vector<std::string> tilesetVec;
    tilesetVec.reserve(200);

    while (pSpriteElement != NULL) {
      ///get frame data for each state
      std::string n = pSpriteElement->Attribute("n");
      tilesetVec.emplace_back(n);
      pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
    }
    tilesetVec.shrink_to_fit();
    return tilesetVec;
  }

  void Load_Entities() {
    Game_Objects_Lists::tilesets["forest_summer"] = Load_Tileset(Entity_Loader::Get_Tileset_Path("forest_summer").c_str());
    Game_Objects_Lists::tilesets["forest_winter"] = Load_Tileset(Entity_Loader::Get_Tileset_Path("forest_winter").c_str());
    Game_Objects_Lists::tilesets["beach"] = Load_Tileset(Entity_Loader::Get_Tileset_Path("beach").c_str());
    Game_Objects_Lists::tilesets["dead"] = Load_Tileset(Entity_Loader::Get_Tileset_Path("dead").c_str());

    Game_Objects_Lists::tilesets["bloodPool"] = Entity_Loader::Get_Names_Of_SubType("neutral", "prop", "blood", "background");
    Game_Objects_Lists::tilesets["bloodSplatter"] = Entity_Loader::Get_Names_Of_SubType("neutral", "prop", "blood", "foreground");

    Game_Objects_Lists::units["wolves"] = Entity_Loader::Get_Unit_Subtypes("beast", "wolf");
    Game_Objects_Lists::units["demons"] = Entity_Loader::Get_Unit_Subtypes("demon", "Demon");
    Game_Objects_Lists::units["eldritch"] = Entity_Loader::Get_Unit_Subtypes("eldritch", "eldritch");
    Game_Objects_Lists::units["troll"] = Entity_Loader::Get_Unit_Subtypes("monster", "troll");
    Game_Objects_Lists::units["amber_slime"] = Entity_Loader::Get_Unit_Subtypes("monster", "amber_slime");
    Game_Objects_Lists::units["red_slime"] = Entity_Loader::Get_Unit_Subtypes("monster", "red_slime");
    Game_Objects_Lists::units["misc_slime"] = Entity_Loader::Get_Unit_Subtypes("monster", "misc_slime");
    Game_Objects_Lists::units["green_slime"] = Entity_Loader::Get_Unit_Subtypes("monster", "green_slime");
    Game_Objects_Lists::units["clear_slime"] = Entity_Loader::Get_Unit_Subtypes("monster", "clear_slime");
    Game_Objects_Lists::units["blue_slime"] = Entity_Loader::Get_Unit_Subtypes("monster", "blue_slime");
  }
}

