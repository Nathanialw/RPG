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

  std::string Append_Quotes(std::string &string) {
    return "'" + string + "'";
  }

  std::string Convert_Char(std::string column, const unsigned char *e) {
    const char *f = (const char *) e;
    if (f == NULL) {
//      Utilities::Log("db::Convert_Char() \"" + column + "\" from DB NULL value, passthrough error");
      return "";
    }
    return std::string(reinterpret_cast< const char *> (e));
  }
}