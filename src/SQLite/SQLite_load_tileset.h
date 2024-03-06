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
    objectTypes.reserve(200);
    sqlite3_stmt *stmt0;

    std::string query0 = "SELECT DISTINCT subtype FROM building_exteriors WHERE tileset = " + tilesetStr;
    sqlite3_prepare_v2(db::db, query0.c_str(), -1, &stmt0, nullptr);
    while (sqlite3_step(stmt0) != SQLITE_DONE) {
      auto text = sqlite3_column_text(stmt0, 0);
      objectTypes.emplace_back(db::Get_String(text));
    }

    for (const auto &subtype: objectTypes) {
      const std::string subtypeStr = db::Append_Quotes(subtype);

      std::vector<std::string> tilesetVec;
      tilesetVec.reserve(200);
      sqlite3_stmt *stmt1;
      std::string query1 = "SELECT name FROM building_exteriors WHERE tileset = " + tilesetStr + " AND subtype = " + subtypeStr;
      sqlite3_prepare_v2(db::db, query1.c_str(), -1, &stmt1, nullptr);
      while (sqlite3_step(stmt1) != SQLITE_DONE) {
        auto text = sqlite3_column_text(stmt1, 0);
        tilesetVec.emplace_back(db::Get_String(text));
      }
      tilesetVec.shrink_to_fit();
      Game_Objects_Lists::tilesetObjects[tileset][subtype] = tilesetVec;
    }
  }


}// namespace SQLite_Load_Tileset
