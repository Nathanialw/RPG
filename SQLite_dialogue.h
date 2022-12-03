#include <string.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "db.h"
#include "utilities.h"


namespace SQLite_Dialogue {

    std::unordered_map<std::string, std::vector<std::string>> dialogue;


    void Load_Dialogue() {// needs to search for  a specific row that I can input in the arguments
        //check if the name exists??
//        std::string type = db::Append_Quotes(text_type);
        Utilities::Log("start loading dialogue");

        sqlite3_stmt *stmt;
        char buf[100];
        const char *jj = "SELECT type, text FROM dialogue";
        strcpy(buf, jj);
//        strcat(buf, type.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
//            get the index of the enum of Item_Components::Item_Type
            auto d = sqlite3_column_text(stmt, 0); //0 only increments up when calling more than one column
            const char *f = (const char *) d;
            std::string type = std::string(reinterpret_cast< const char *> (f));

            auto g = sqlite3_column_text(stmt, 1); //0 only increments up when calling more than one column
            const char *h = (const char *) g;
            std::string text = std::string(reinterpret_cast< const char *> (h));

            dialogue[type].emplace_back(text);
        }

        for (auto text : dialogue){
            text.second.shrink_to_fit();
            Utilities::Log(text.second.size());
        }
        Utilities::Log("Loading dialogue success!");
    }

    void Init_Dialogue() {
        Load_Dialogue();
    }

};