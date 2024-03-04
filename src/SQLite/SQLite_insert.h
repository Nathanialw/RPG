#pragma once
#include "db.h"
#include "sqlite/sqlite3.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace SQLite_Insert {

  void Update_DB(const std::string &table, const std::string field, const std::string &name, const float value) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    const std::string tableStr = db::Append_Quotes(table);
    const std::string fieldStr = db::Append_Quotes(field);
    const std::string valueStr = db::Append_Quotes(value);
    const std::string unit_name = db::Append_Quotes(name);
    char *error;

    std::string sql("UPDATE " + tableStr + " SET " + fieldStr + " = " + valueStr + " WHERE name = " + unit_name);
    sqlite3_exec(db::db, sql.c_str(), nullptr, nullptr, &error);
  }
}// namespace SQLite_Insert