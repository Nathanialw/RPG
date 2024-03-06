#pragma once
#include "cstring"
#include "db.h"
#include "game_objects.h"
#include "string"
#include "vector"

namespace SQLite_Load_Tileset {

  void Load(std::string tileset) {
    const std::string tilesetStr = db::Append_Quotes(tileset);

    std::vector<std::string> objectTypes;
    objectTypes.reserve(50);
    sqlite3_stmt *stmt0;

    //returns subtypes available in the labyrinth
    std::string query0 = "SELECT DISTINCT subtype FROM building_exteriors WHERE tileset = " + tilesetStr;
    sqlite3_prepare_v2(db::db, query0.c_str(), -1, &stmt0, nullptr);
    while (sqlite3_step(stmt0) != SQLITE_DONE) {
      auto text = sqlite3_column_text(stmt0, 0);
      objectTypes.emplace_back(db::Get_String(text));
    }

    // returns names objects available in the labyrinth
    std::vector<std::string> objectNames;
    objectNames.reserve(50);
    sqlite3_stmt *stmt2;
    std::string query2 = "SELECT DISTINCT object_name FROM building_exteriors WHERE tileset = " + tilesetStr;
    sqlite3_prepare_v2(db::db, query2.c_str(), -1, &stmt2, nullptr);
    while (sqlite3_step(stmt2) != SQLITE_DONE) {
      auto text = sqlite3_column_text(stmt2, 0);
      objectNames.emplace_back(db::Get_String(text));
    }

    // saves each object in vectors holding all available directions
    for (const std::string &subtype: objectTypes) {
      const std::string subtypeStr = db::Append_Quotes(subtype);
      for (const std::string &object: objectNames) {
        const std::string objectStr = db::Append_Quotes(object);

        std::vector<std::string> tilesetVec;
        tilesetVec.reserve(16);
        sqlite3_stmt *stmt1;
        std::string query1 = "SELECT name FROM building_exteriors WHERE tileset = " + tilesetStr + " AND subtype = " + subtypeStr + " AND object_name = " + objectStr;
        sqlite3_prepare_v2(db::db, query1.c_str(), -1, &stmt1, nullptr);
        while (sqlite3_step(stmt1) != SQLITE_DONE) {
          auto text = sqlite3_column_text(stmt1, 0);
          tilesetVec.emplace_back(db::Get_String(text));
        }

        if (!tilesetVec.empty()) {
          tilesetVec.shrink_to_fit();
          Game_Objects_Lists::tilesetObjects[tileset][subtype][object] = tilesetVec;
        }
      }
    }
  }


}// namespace SQLite_Load_Tileset
