#pragma once
#include "db.h"
#include "stdio.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <string.h>

namespace SQLite_Spell_Data {

  struct Spell_Data {
    std::string sheetType = "";
    std::string xmlPath = "";
    std::string imagePath = "";
    std::string casting_xml_path = "";
    std::string casting_image_path = "";
    std::string hit_xml_path = "";
    std::string hit_image_path = "";
    float range = 500.0f;
  };

  std::string Get_db_Text(sqlite3_stmt *stmt, int i) {
    auto path = sqlite3_column_text(stmt, i);//0 only increments up when calling more than one column
    if (path) {
      const char *d = (const char *) path;
      return std::string(reinterpret_cast<const char *>(d));
    }
    return "";
  }

  Spell_Data Spell_Loader(std::string name) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    std::string unit_name = db::Append_Quotes(name);

    Spell_Data data;
    sqlite3_stmt *stmt;
    char buf[400];
    const char *jj = "SELECT range, sheet_type, image_path, xml_path, casting_xml_path, casting_image_path, hit_xml_path, hit_image_path FROM spell_data WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, unit_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      data.range = (float) sqlite3_column_double(stmt, 0);//0 only increments up when calling more than one column
      data.sheetType = Get_db_Text(stmt, 1);
      data.imagePath = Get_db_Text(stmt, 2);
      data.xmlPath = Get_db_Text(stmt, 3);
      data.casting_xml_path = Get_db_Text(stmt, 4);
      data.casting_image_path = Get_db_Text(stmt, 5);
      data.hit_xml_path = Get_db_Text(stmt, 6);
      data.hit_image_path = Get_db_Text(stmt, 7);
    }
    return data;
  }
}// namespace SQLite_Spell_Data