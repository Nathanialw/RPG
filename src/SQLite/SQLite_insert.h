#include "db.h"
#include "sqlite/sqlite3.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace SQLite_Insert {

  void Update_Offset(const std::string &name, const float x, const float y) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    const std::string unit_name = db::Append_Quotes(name);
    const std::string xStr = db::Append_Quotes(x);
    const std::string yStr = db::Append_Quotes(y);
    char *error;

    std::string sql("UPDATE building_exteriors SET x_offset = " + xStr + " WHERE name = " + unit_name);
    sqlite3_exec(db::db, sql.c_str(), nullptr, nullptr, &error);
    Utilities::Log(error);
    std::string sql0("UPDATE building_exteriors SET y_offset = " + yStr + " WHERE name = " + unit_name);
    sqlite3_exec(db::db, sql0.c_str(), nullptr, nullptr, &error);
    Utilities::Log(error);
  }
}// namespace SQLite_Insert