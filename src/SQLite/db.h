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

  bool Get_bool(std::string column, int value) {
    if (value == 0) {
      return false;
    }
    return true;
  }
}// namespace db