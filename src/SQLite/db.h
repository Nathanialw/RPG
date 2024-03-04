#pragma once

#include "sqlite/sqlite3.h"

namespace db {
  sqlite3 *db;

  struct Unit_Data {
    std::string name = "";
    std::string imgPath = "";
    std::string portraitPath = "";
    std::string bodyPath = "";
  };

  void Init() {
    const char *db_filepath = "Data/db/data.db";
    int error = sqlite3_open(db_filepath, &db::db);
    if (error) {
      //if error then display error and close connection
      std::cout << "DB Open Error: " << sqlite3_errmsg(db::db) << std::endl;
      sqlite3_close(db::db);
    } else {
      std::cout << "Opened Database Successfully!" << std::endl;
    }
  }

  std::string Append_Quotes(const std::string &string) {
    return "'" + string + "'";
  }

  std::string Append_Quotes(const int &num) {
    return "'" + std::to_string(num) + "'";
  }

  std::string Append_Quotes(const float &num) {
    return "'" + std::to_string(num) + "'";
  }

  std::string Convert_Char(std::string column, const unsigned char *e) {
    const char *f = (const char *) e;
    if (f == NULL) {
      //      Utilities::Log("db::Convert_Char() \"" + column + "\" from DB NULL value, passthrough error");
      return "";
    }
    return std::string(reinterpret_cast<const char *>(e));
  }

  std::string Get_String(const unsigned char *text) {
    const char *s;
    s = (const char *) text;
    if (s != NULL) {
      return std::string(reinterpret_cast<const char *>(s));
    }
    return "";
  }

  bool Get_bool(std::string column, int value) {
    if (value == 0) {
      return false;
    }
    return true;
  }
}// namespace db