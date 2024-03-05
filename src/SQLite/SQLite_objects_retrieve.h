#pragma once
#include "base_structs.h"
#include "cstring"
#include "db.h"
#include "string"

namespace SQLite_Object_Retrieve {

  f2 Get_Building_Data(const std::string &name) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    const std::string unit_name = db::Append_Quotes(name);
    f2 offset{};

    const unsigned char *text = nullptr;
    sqlite3_stmt *stmt0;
    char buf0[300];
    const char *jj0 = "SELECT name FROM building_exteriors WHERE interior = ";
    strcpy(buf0, jj0);
    strcat(buf0, unit_name.c_str());
    sqlite3_prepare_v2(db::db, buf0, -1, &stmt0, nullptr);
    while (sqlite3_step(stmt0) != SQLITE_DONE) {
      text = sqlite3_column_text(stmt0, 0);
    }

    if (text) {
      sqlite3_stmt *stmt;
      char buf[300];
      const char *jj = "SELECT x_collider_offset, y_collider_offset FROM building_exteriors WHERE interior = ";
      strcpy(buf, jj);
      strcat(buf, unit_name.c_str());
      sqlite3_prepare_v2(db::db, buf, -1, &stmt, nullptr);
      while (sqlite3_step(stmt) != SQLITE_DONE) {
        offset.x = (float) sqlite3_column_double(stmt, 0);
        offset.y = (float) sqlite3_column_double(stmt, 1);
      }
    }

    else {
      sqlite3_stmt *stmt;
      char buf[300];
      const char *jj = "SELECT x_collider_offset, y_collider_offset FROM building_exteriors WHERE name = ";
      strcpy(buf, jj);
      strcat(buf, unit_name.c_str());
      sqlite3_prepare_v2(db::db, buf, -1, &stmt, nullptr);
      while (sqlite3_step(stmt) != SQLITE_DONE) {
        offset.x = (float) sqlite3_column_double(stmt, 0);
        offset.y = (float) sqlite3_column_double(stmt, 1);
      }
    }
    return offset;
  }


  std::string Get_Interior(const std::string &name) {
    const std::string unit_name = db::Append_Quotes(name);

    sqlite3_stmt *stmt0;
    char buf0[300];
    const char *jj0 = "SELECT interior FROM building_exteriors WHERE name = ";
    strcpy(buf0, jj0);
    strcat(buf0, unit_name.c_str());
    sqlite3_prepare_v2(db::db, buf0, -1, &stmt0, nullptr);
    while (sqlite3_step(stmt0) != SQLITE_DONE) {
      auto text = sqlite3_column_text(stmt0, 0);
      return db::Get_String(text);
    }
  }
}// namespace SQLite_Object_Retrieve