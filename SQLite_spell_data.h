#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include "db.h"
#include <string.h>
#include "stdio.h"

namespace SQLite_Spell_Data {

    struct Spell_Data {
        std::string path = "";
    };

    Spell_Data Spell_Loader(std::string name) {// needs to search for  a specific row that I can input in the arguments
        //check if the name exists??
        std::string unit_name = db::Append_Quotes(name);


        Spell_Data data;
        sqlite3_stmt *stmt;
        char buf[400];
        const char *jj = "SELECT image_path FROM spell_data WHERE name = ";
        strcpy(buf, jj);
        strcat(buf, unit_name.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            auto path = sqlite3_column_text(stmt, 0); //0 only increments up when calling more than one column
            const char *s = (const char *) path;
            data.path = std::string(reinterpret_cast< const char *> (s));
        }
        return data;
    }
}