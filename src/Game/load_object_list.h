#pragma once

#include <array>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "Objects/Social/social_control.h"
#include "SQLite_load_tileset.h"
#include "SQLite_unit_data.h"
#include "components.h"
#include "game_objects.h"
#include "tinyxml/tinyxml2.h"
#include "utilities.h"

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
        if (typeStr == "unit")
            return Component::Entity_Type::unit;
        if (typeStr == "foliage")
            return Component::Entity_Type::foliage;
        if (typeStr == "spell")
            return Component::Entity_Type::spell;
        if (typeStr == "item")
            return Component::Entity_Type::item;
        if (typeStr == "object")
            return Component::Entity_Type::object;
        if (typeStr == "prop")
            return Component::Entity_Type::prop;
        if (typeStr == "building")
            return Component::Entity_Type::building;

        std::cout << "Type: " << typeStr << " from db not found; aborting. " << std::endl;
        return Component::Entity_Type::SIZE;
    }

    std::vector<std::string> Get_Entities(std::string &typeStr) {
        if (typeStr == "unit") {
            Utilities::Log("units");
            return Entity_Loader::Get_All_Units_Of_Type(typeStr);
        }
        if (typeStr == "foliage") {
            Utilities::Log("foliage");
            return Entity_Loader::Get_All_Units_Of_Type(typeStr);;
        }
        if (typeStr == "building") {
            Utilities::Log("building");
            return Entity_Loader::Get_All_Objects_Of_Type(typeStr);;
        }
        if (typeStr == "spell") {
            Utilities::Log("spells");
            return Entity_Loader::Get_All_Units_Of_Type(typeStr);
        }
        if (typeStr == "item") {
            Utilities::Log("items");
            return Entity_Loader::Get_All_Items_Of_Type(typeStr);
        }
        if (typeStr == "object") {
            Utilities::Log("objects");
            return Entity_Loader::Get_All_Objects_Of_Type(typeStr);
        }
        if (typeStr == "prop") {
            Utilities::Log("props");
            return Entity_Loader::Get_All_Objects_Of_Type(typeStr);
        }

        std::cout << "Type: " << typeStr << " from db not found; aborting. " << std::endl;
        std::vector<std::string> empty;
        return empty;
    }

    using namespace Social_Component;
    using namespace Component;

    std::vector<std::string> Get_Buildings(const Social_Component::Race &race, const Component::Entity_Type &type, const Component::Buildings &buildingType) {
        return gameObjects[(int) type][(int) race][(int) buildingType];
    };

    std::vector<std::string> Get_Props(const Social_Component::Race &race, const Component::Entity_Type &type, const Component::Props &propType) {
        return gameObjects[(int) type][(int) race][(int) propType];
    };

    std::vector<std::string> Get_Units(const Social_Component::Race &race, const Component::Entity_Type &type, const Component::Units &unitType) {
        return gameObjects[(int) type][(int) race][(int) unitType];
    };

    void Load_Tileset(const std::string &tileset) {
        std::string xmlPath = Entity_Loader::Get_Tileset_Path(tileset);

        tinyxml2::XMLDocument spriteSheetData;
        spriteSheetData.LoadFile(xmlPath.c_str());
        tinyxml2::XMLElement *pSpriteElement;
        pSpriteElement = spriteSheetData.RootElement()->FirstChildElement("sprite");
        //    std::string imgPath = spriteSheetData.RootElement()->Attribute("imagePath");

        std::vector<std::string> tilesetVec;
        tilesetVec.reserve(200);

        int i = 0;
        while (pSpriteElement != nullptr) {
            ///get frame data for each state
            std::string n = pSpriteElement->Attribute("n");
            tilesetVec.emplace_back(n);
            pSpriteElement = pSpriteElement->NextSiblingElement("sprite");
            Game_Objects_Lists::tilesetObjectIndexes[tileset][n] = i;
            i++;
        }
        tilesetVec.shrink_to_fit();
        Game_Objects_Lists::tilesets[tileset] = tilesetVec;
    }

    void Load_Entities() {
        Load_Tileset("forest_summer");
        Load_Tileset("forest_winter");
        Load_Tileset("beach");
        Load_Tileset("dead");
        Load_Tileset("hell");
        Load_Tileset("cave_entrances");
        Load_Tileset("labyrinth");
        SQLite_Load_Tileset::Load("labyrinth");


        Game_Objects_Lists::tilesets["buildings_orc"] = Entity_Loader::Get_Names_Of_SubType("orc", "building", "house");
        for (int i = 0; i < Game_Objects_Lists::tilesets["buildings_orc"].size(); ++i) {
            Game_Objects_Lists::indexes["buildings_orc"][Game_Objects_Lists::tilesets["buildings_orc"][i]] = i;
        }

        Game_Objects_Lists::tilesets["buildings_human"] = Entity_Loader::Get_Names_Of_SubType("human", "building", "house");
        for (int i = 0; i < Game_Objects_Lists::tilesets["buildings_human"].size(); ++i) {
            Game_Objects_Lists::indexes["buildings_human"][Game_Objects_Lists::tilesets["buildings_human"][i]] = i;
        }

        Game_Objects_Lists::tilesets["bloodPool"] = Entity_Loader::Get_Names_Of_SubType("neutral", "prop", "blood", "background");
        Game_Objects_Lists::tilesets["bloodSplatter"] = Entity_Loader::Get_Names_Of_SubType("neutral", "prop", "blood", "foreground");

        Game_Objects_Lists::units["wolves"] = Entity_Loader::Get_Unit_Subtypes("beast", "wolf");
        Game_Objects_Lists::units["naga"] = Entity_Loader::Get_Unit_Subtypes("monster", "naga");

        Game_Objects_Lists::units["zombies"] = Entity_Loader::Get_Unit_Subtypes("beast", "wolf");
        Game_Objects_Lists::units["skeletons"] = Entity_Loader::Get_Unit_Subtypes("beast", "wolf");

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


}// namespace Load_Object_List
