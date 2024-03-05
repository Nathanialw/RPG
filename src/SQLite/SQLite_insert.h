#pragma once
#include "db.h"
#include "sqlite/sqlite3.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace SQLite_Insert {

  void Update_DB(const std::string &table, const std::string &field, const std::string &name, const float &value) {// needs to search for  a specific row that I can input in the arguments
    const std::string tableStr = db::Append_Quotes(table);
    const std::string fieldStr = db::Append_Quotes(field);
    const std::string unit_name = db::Append_Quotes(name);
    char *error;
    float DB_value = 0;


    //    sqlite3_stmt *stmt0;
    //    char buf0[300];
    //    const char *jj0 = "SELECT ";
    //    strcpy(buf0, jj0);
    //    strcat(buf0, fieldStr.c_str());
    //    const char *kk0 = " FROM ";
    //    strcat(buf0, kk0);
    //    strcat(buf0, tableStr.c_str());
    //    const char *ll0 = " WHERE name = ";
    //    strcat(buf0, ll0);
    //    strcat(buf0, unit_name.c_str());
    //    sqlite3_prepare_v2(db::db, buf0, -1, &stmt0, nullptr);
    //
    //    while (sqlite3_step(stmt0) != SQLITE_DONE) {
    //      DB_value = (float) sqlite3_column_double(stmt0, 0);
    //      Utilities::Log(DB_value);
    //    }
    //
    //    sqlite3_stmt *stmt;
    //    char buf[300];
    //    const char *jj = "SELECT collider_type, radius, x_offset, y_offset, sprite_layout, xml, img, race, whole_sprite, interior, direction FROM building_exteriors WHERE name = ";
    //    strcpy(buf, jj);
    //    strcat(buf, unit_name.c_str());
    //    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    //    while (sqlite3_step(stmt) != SQLITE_DONE) {
    //
    //
    //    sqlite3_stmt *stmt;
    //    std::string buf = "SELECT " + fieldStr + " FROM " + tableStr + " WHERE name = " + unit_name;
    //    sqlite3_prepare_v2(db::db, buf.c_str(), -1, &stmt, nullptr);
    //    while (sqlite3_step(stmt) != SQLITE_DONE) {
    //      DB_value = (float) sqlite3_column_double(stmt, 0);
    //      Utilities::Log(DB_value);
    //    }


    sqlite3_stmt *stmt2;
    std::string buf2 = "SELECT " + field + "  FROM " + table + " WHERE name = " + unit_name;
    sqlite3_prepare_v2(db::db, buf2.c_str(), -1, &stmt2, nullptr);
    while (sqlite3_step(stmt2) != SQLITE_DONE) {
      DB_value = (float) sqlite3_column_double(stmt2, 0);
    }

    DB_value += value;
    const std::string DB_valueStr = db::Append_Quotes(DB_value);

    //check if the name exists??

    std::string sql("UPDATE " + tableStr + " SET " + fieldStr + " = " + DB_valueStr + " WHERE name = " + unit_name);
    sqlite3_exec(db::db, sql.c_str(), nullptr, nullptr, &error);
  }
}// namespace SQLite_Insert