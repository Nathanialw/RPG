#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include "db.h"
#include <string.h>
#include "vector"
#include "item_components.h"

namespace SQLite_Item_Data {

    // read in all items and emplace each unit type into the map and fill it up
    //  holds a map for each unit type >>>  holds a vector for each item type for the unit >>> the vector is a list of strings of the name of each item in the db
    std::unordered_map<Item_Component::Unit_Equip_Type, std::unordered_map<Item_Component::Item_Type, std::vector<std::string>>>Items;
//    std::array<std::array<std::vector<std::string>, (int)Item_Component::Item_Type::size>, (int)Item_Component::Unit_Equip_Type::size>Items;

    void Load_Item_Names() {// needs to search for  a specific row that I can input in the arguments
        //check if the name exists??
//        std::string unit_name = db::Append_Quotes(unit_type);

        sqlite3_stmt *stmt;
        char buf[1000];
        const char *jj = "SELECT slot, type, equip_type FROM weapon_types";
        strcpy(buf, jj);
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
//            get the index of the enum of Item_Components::Item_Type
            auto itemType = sqlite3_column_text(stmt, 0); //0 only increments up when calling more than one column
            const char *f = (const char *) itemType;
            if (f == NULL) {
                Utilities::Log("Load_Item_Names() 'slot' from DB NULL value, passthrough error");
                continue;
            }
            std::string g = std::string(reinterpret_cast< const char *> (f));
            Item_Component::Item_Type item_type = Item_Component::Get_Item_Type(g);

            //get the name of the item as a sting
            auto type = sqlite3_column_text(stmt, 1); //0 only increments up when calling more than one column
            const char *s = (const char *) type;
            std::string item_name = std::string(reinterpret_cast< const char *> (s));

            // get the RTP_male, classes_female etc strings
            auto db_equip_type = sqlite3_column_text(stmt, 2); //0 only increments up when calling more than one column
            const char *d = (const char *) db_equip_type;
            if (d == NULL) {
                Utilities::Log("Load_Item_Names() 'equip_type' from DB NULL value, passthrough error");
                continue;
            }
            std::string equip_type = std::string(reinterpret_cast< const char *> (d));
            Item_Component::Unit_Equip_Type unit_equip_type = Item_Component::Get_Unit_Equip_Type(equip_type);

            Items[unit_equip_type][item_type].emplace_back(item_name);
        }
//        Utilities::Log(Items.size());
//        Utilities::Log("Loading items success!");
    }
}