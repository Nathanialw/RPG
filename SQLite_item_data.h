#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include "db.h"
#include <string.h>
#include "stdio.h"
#include "vector"

namespace SQLite_Item_Data {

    std::vector<std::string> legs;
    std::vector<std::string> chest;
    std::vector<std::string> helm;

    std::vector<std::string> mace;
    std::vector<std::string> sword;
    std::vector<std::string> polearm;
    std::vector<std::string> axe;

    void Load_Item_Names(std::string unit_type) {// needs to search for  a specific row that I can input in the arguments
        //check if the name exists??
        std::string unit_name = db::Append_Quotes(unit_type);

        sqlite3_stmt *stmt;
        char buf[400];
        const char *jj = "SELECT item_type, type FROM weapon_types WHERE unit_type = ";
        strcpy(buf, jj);
        strcat(buf, unit_name.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            auto type = sqlite3_column_text(stmt, 0); //0 only increments up when calling more than one column
            const char *d = (const char *) type;
            std::string vec = std::string(reinterpret_cast< const char *> (d));

            auto itemType = sqlite3_column_text(stmt, 1); //0 only increments up when calling more than one column
            const char *s = (const char *) itemType;

            if (vec == "legs") {
                legs.emplace_back(std::string(reinterpret_cast< const char *> (s)));
            }
            else if (vec == "chest") {
                chest.emplace_back(std::string(reinterpret_cast< const char *> (s)));
            }
            else if (vec == "helm") {
                helm.emplace_back(std::string(reinterpret_cast< const char *> (s)));
            }
            else if (vec == "mace") {
                mace.emplace_back(std::string(reinterpret_cast< const char *> (s)));
            }
            else if (vec == "sword") {
                sword.emplace_back(std::string(reinterpret_cast< const char *> (s)));
            }
            else if (vec == "polearm") {
                polearm.emplace_back(std::string(reinterpret_cast< const char *> (s)));
            }
            else if (vec == "axe") {
                axe.emplace_back(std::string(reinterpret_cast< const char *> (s)));
            }


        }
        legs.shrink_to_fit();
        chest.shrink_to_fit();
        helm.shrink_to_fit();
        mace.shrink_to_fit();
        sword.shrink_to_fit();
        polearm.shrink_to_fit();
        axe.shrink_to_fit();
        std::cout << legs.size() << std::endl;
    }

}