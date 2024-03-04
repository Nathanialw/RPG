#pragma once
#include "base_structs.h"
#include "cstring"
#include "db.h"
#include "string"

namespace SQLite_Object_Retrieve {

  f2 Get_Building_Data(const std::string &name) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    const std::string unit_name = db::Append_Quotes(name);
    f2 offset;

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
    return offset;
  }
}// namespace SQLite_Object_Retrieve