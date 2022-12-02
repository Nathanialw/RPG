#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include "db.h"
#include <string.h>
#include "stdio.h"
#include "vector"
#include "item_components.h"

namespace SQLite_Item_Data {

    // read in all items and emplace each unit type into the map and fill it up
    //  holds a map for each unit type >>>  holds a vector for each item type for the unit >>> the vector is a list of strings of the name of each item in the db
    std::unordered_map<std::string, std::unordered_map<Item_Component::Item_Type, std::vector<std::string>>>Items;

    Item_Component::Item_Type Get_Item_Type(std::string &db_type) {
       if (db_type == "back") {
            return Item_Component::Item_Type::back;
       }
       else if (db_type == "mainhand") {
            return Item_Component::Item_Type::mainhand;
       }
       else if (db_type == "amulet") {
            return Item_Component::Item_Type::amulet;
       }
       else if (db_type == "face") {
            return Item_Component::Item_Type::face;
       }
       else if (db_type == "belt") {
            return Item_Component::Item_Type::belt;
       }
       else if (db_type == "boots") {
            return Item_Component::Item_Type::boots;
       }
       else if (db_type == "ranged") {
            return Item_Component::Item_Type::ranged;
       }
       else if (db_type == "chest") {
            return Item_Component::Item_Type::chest;
       }
       else if (db_type == "clothes") {
            return Item_Component::Item_Type::clothes;
       }
       else if (db_type == "crown") {
            return Item_Component::Item_Type::crown;
       }
       else if (db_type == "dirt") {
            return Item_Component::Item_Type::dirt;
       }
       else if (db_type == "gloves") {
            return Item_Component::Item_Type::gloves;
       }
       else if (db_type == "hair") {
            return Item_Component::Item_Type::hair;
       }
       else if (db_type == "helm") {
            return Item_Component::Item_Type::helm;
       }
       else if (db_type == "hood") {
            return Item_Component::Item_Type::hood;
       }
       else if (db_type == "kilt") {
            return Item_Component::Item_Type::kilt;
       }
       else if (db_type == "legs") {
            return Item_Component::Item_Type::legs;
       }
       else if (db_type == "ring") {
            return Item_Component::Item_Type::ring;
       }
       else if (db_type == "offhand") {
            return Item_Component::Item_Type::offhand;
       }
       else if (db_type == "shins") {
            return Item_Component::Item_Type::shins;
       }
       else if (db_type == "shoulders") {
            return Item_Component::Item_Type::shoulders;
       }
       else if (db_type == "wrist") {
           return Item_Component::Item_Type::wrist;
       }
       else if (db_type == "jewelry") {
           return Item_Component::Item_Type::jewelry;
       }
       else {
           Utilities::Log("Get_Item_Type(std::string &db_type) passthrough error");
           return Item_Component::Item_Type::mainhand;
       }
    }

    void Load_Item_Names() {// needs to search for  a specific row that I can input in the arguments
        //check if the name exists??
//        std::string unit_name = db::Append_Quotes(unit_type);

        sqlite3_stmt *stmt;
        char buf[400];
        const char *jj = "SELECT slot, type, equip_type FROM weapon_types";
        strcpy(buf, jj);
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
//            get the index of the enum of Item_Components::Item_Type
            auto itemType = sqlite3_column_text(stmt, 0); //0 only increments up when calling more than one column
            const char *f = (const char *) itemType;
            std::string g = std::string(reinterpret_cast< const char *> (f));
            Utilities::Log(g);
            Item_Component::Item_Type item_type = Get_Item_Type(g);

            //get the name of the item as a sting
            auto type = sqlite3_column_text(stmt, 1); //0 only increments up when calling more than one column
            const char *s = (const char *) type;
            std::string item_name = std::string(reinterpret_cast< const char *> (s));
            Utilities::Log(item_name);

            // get the RTP_male, classes_female etc strings
            auto equip_type = sqlite3_column_text(stmt, 2); //0 only increments up when calling more than one column
            const char *d = (const char *) equip_type;
            std::string vec = std::string(reinterpret_cast< const char *> (d));
            Utilities::Log(vec);

            Items[vec][item_type].emplace_back(item_name);
        }
        Utilities::Log(Items.size());
        Utilities::Log("Loading items success!");
    }
}