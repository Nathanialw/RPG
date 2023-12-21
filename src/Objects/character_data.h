#pragma once
#include "components.h"
#include "db.h"
#include <vector>

namespace Character_Data {
  std::vector<std::string> firstNames;
  std::vector<std::string> lastNames;

  void Get_Names_From_DB() {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    //        std::string unit_name = db::Append_Quotes(name);

    sqlite3_stmt *stmt;
    char buf[400];
    const char *jj = "SELECT first_names, last_names FROM name_data";
    strcpy(buf, jj);
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      auto first = sqlite3_column_text(stmt, 0);//0 only increments up when calling more than one column
      const char *s = (const char *) first;
      if (s) {
        firstNames.emplace_back(std::string(reinterpret_cast<const char *>(s)));
      }

      auto last = sqlite3_column_text(stmt, 1);//0 only increments up when calling more than one column
      const char *d = (const char *) last;
      if (d) {
        lastNames.emplace_back(std::string(reinterpret_cast<const char *>(d)));
      }
    }
    firstNames.shrink_to_fit();
    lastNames.shrink_to_fit();
  }

  void Get_Name(Component::Name &name) {
    if (firstNames.size() > 0) {
      name.first = firstNames[rand() % firstNames.size()];
    } else {
      name.first = "John";
    }
    if (lastNames.size() > 0) {
      name.last = firstNames[rand() % lastNames.size()];
    } else {
      name.last = "Smith";
    }
  }
}// namespace Character_Data
