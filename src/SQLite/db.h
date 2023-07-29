#pragma once

#include "../sqlite3.h"

namespace db {
  sqlite3 *db;

  struct Unit_Data {
    std::string name = "";
    std::string imgPath = "";
  };

  std::string Append_Quotes(std::string &string) {
    return "'" + string + "'";
  }
}