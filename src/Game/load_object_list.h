#pragma once
#include <array>
#include <unordered_map>
#include <vector>
#include <string>
#include <map>
#include "SQLite_unit_data.h"
#include "components.h"
#include "utilities.h"
#include "social_control.h"
#include "game_objects.h"

namespace Load_Object_List {

  std::array<std::vector<std::string> [(int)Social_Component::Race::SIZE][(int)Component::Foliage::SIZE], (int)Component::Entity_Type::SIZE>gameObjects;

  struct Game_Objects {
    std::vector<std::vector<std::string> [(int)Social_Component::Race::SIZE][(int)Component::Entity_Type::SIZE][(int)Component::Foliage::SIZE]>gameObjects;
    std::vector<std::string> spells[(int)Social_Component::Race::SIZE][(int)Component::Entity_Type::SIZE][(int)Component::Spells::SIZE];
    std::vector<std::string> items[(int)Social_Component::Race::SIZE][(int)Component::Entity_Type::SIZE][(int)Component::Items::SIZE];
    std::vector<std::string> objects[(int)Social_Component::Race::SIZE][(int)Component::Entity_Type::SIZE][(int)Component::Objects::SIZE];
    std::vector<std::string> props[(int)Social_Component::Race::SIZE][(int)Component::Entity_Type::SIZE][(int)Component::Props::SIZE];
    std::vector<std::string> buildings[(int)Social_Component::Race::SIZE][(int)Component::Entity_Type::SIZE][(int)Component::Buildings::SIZE];

    //    std::unordered_map<std::string, std::vector<std::string>> gameObjects; 
  };

  Component::Entity_Type Get_Type(std::string &typeStr) {
    
    if (typeStr == "unit") {
      return Component::Entity_Type::unit;
    }
    else if (typeStr == "foliage") {
      return Component::Entity_Type::foliage;
    }
    else if (typeStr == "spell") {
      return Component::Entity_Type::spell;
    }
    else if (typeStr == "item") {
      return Component::Entity_Type::item;
    }
    else if (typeStr == "object") {
      return Component::Entity_Type::object;
    }
    else if (typeStr == "prop") {
      return Component::Entity_Type::prop;
    }
    else if (typeStr == "building") {
      return Component::Entity_Type::building;
    }
    std::cout << "Type: " << typeStr << " from db not found; aborting. " << std::endl;
    return Component::Entity_Type::SIZE;
  }

  int Get_Subtype (std::string &type) {




    
    return 1;
  };

  std::vector<std::string> Get_Entities(std::string &typeStr) {
    if (typeStr == "unit") {
      Utilities::Log("units");
      return Entity_Loader::Get_All_Units_Of_Type(typeStr);
    }
    else if (typeStr == "foliage") {
      Utilities::Log("foliage");
      return Entity_Loader::Get_All_Units_Of_Type(typeStr);;
    }
    else if (typeStr == "building") {
      Utilities::Log("building");
      return Entity_Loader::Get_All_Objects_Of_Type(typeStr);;
    }
    else if (typeStr == "spell") {
      Utilities::Log("spells");
      return Entity_Loader::Get_All_Units_Of_Type(typeStr);
    }
    else if (typeStr == "item") {
      Utilities::Log("items");
      return Entity_Loader::Get_All_Items_Of_Type(typeStr);
    }
    else if (typeStr == "object") {
      Utilities::Log("objects");
      return Entity_Loader::Get_All_Objects_Of_Type(typeStr);
    }
    else if (typeStr == "prop") {
      Utilities::Log("props");
      return Entity_Loader::Get_All_Objects_Of_Type(typeStr);
    }
    std::cout << "Type: " << typeStr << " from db not found; aborting. " << std::endl;
    std::vector<std::string> empty;
    return empty;
  }
  
  // race >> type >> subtype

  using namespace Social_Component;
  using namespace Component;
  
  std::vector<std::string> Get_Buildings(Social_Component::Race race, Component::Entity_Type type, Component::Buildings buildingType) {    
    return gameObjects[(int)type][(int)race][(int)buildingType];
  };

  std::vector<std::string> Get_Props(Social_Component::Race race, Component::Entity_Type type, Component::Props propType) {    
    return gameObjects[(int)type][(int)race][(int)propType];
  };

  std::vector<std::string> Get_Units(Social_Component::Race race, Component::Entity_Type type, Component::Units unitType) {    
    return gameObjects[(int)type][(int)race][(int)unitType];
  };
  
  
  void Load_Entities () {
    // auto races = Entity_Loader::Get_All_Of_Races();
    // for (auto race : races) {
    //   Utilities::Log(race); 
    //   auto types = Entity_Loader::Get_All_Types_Of_Race(race);
    //   for (auto type : types) {
    // 	Utilities::Log(type);
    // 	auto subtypes = Entity_Loader::Get_All_Subtype_Of_Type(type);
    // 	for (auto subtype : subtypes) {
    // 	  Utilities::Log(subtype);
    // 	  gameObjects[(int)Get_Type(type)][(int)Social_Control::Get_Race(race)][Get_Subtype(subtype)] = Entity_Loader::Get_All_Names_Of_SubType(subtype);
    // 	}
    //   }
    // }
    Game_Objects_Lists::bloodPoolVec = Entity_Loader::Get_Names_Of_SubType("blood_pool", "neutral");
    Game_Objects_Lists::bloodSplatterVec = Entity_Loader::Get_Names_Of_SubType("blood_splatter", "neutral");

    Utilities::Log("pool");
    for (auto i : Game_Objects_Lists::bloodPoolVec) {
      Utilities::Log(i);
    }
    Utilities::Log("spatter");
    for (auto i : Game_Objects_Lists::bloodSplatterVec) {
      Utilities::Log(i);
    }
  }
  


}

