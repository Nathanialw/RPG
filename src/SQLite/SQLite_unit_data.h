#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "db.h"
#include <string.h>
#include <string>
#include <vector>
#include "stdio.h"

namespace Entity_Loader {
  struct Data {
    float radius = 0.0f;
    float speed = 0.0f;
    float mass = 0.0f;
    int health = 0;
    int damage_min = 0;
    int damage_max = 0;
    float melee_range = 0.0f;
    float attack_speed = 0.0f;
    float sight_radius = 0.0f;
    float scale = 0.0f;
    int body_type = 2;
    float interact_r = 10.0f;
    float interact_h = 10.0f;
    float x_offset = 0.0f;
    float y_offset = 0.0f;
    std::string temp_type_name = "NPC";
    std::string equip_type = "none";
    std::string entity_class = "monster";
    std::string sheet = "none";
    std::string race = "rogue";
  };

  void init_db() {
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

  Data parse_data(std::string name) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    std::string unit_name = db::Append_Quotes(name);
    Data values;
    sqlite3_stmt *stmt;
    char buf[400];
    const char *jj = "SELECT radius, speed, mass, health, damage_min, damage_max, melee_range, attack_speed, sight_radius, scale, body_type, interact_r, interact_h, x_offset, y_offset, equip_type, race, temp_type_name FROM unit_data WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, unit_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      values.radius = (float) sqlite3_column_double(stmt, 0); //0 only increments up when calling more than one column
      values.speed = (float) sqlite3_column_double(stmt, 1); //0 only increments up when calling more than one column
      values.mass = (float) sqlite3_column_double(stmt, 2); //0 only increments up when calling more than one column
      values.health = sqlite3_column_int(stmt, 3);
      values.damage_min = sqlite3_column_int(stmt, 4);
      values.damage_max = sqlite3_column_int(stmt, 5);
      values.melee_range = (float) sqlite3_column_double(stmt, 6);
      values.attack_speed = sqlite3_column_int(stmt, 7);
      values.sight_radius = (float) sqlite3_column_double(stmt, 8);
      values.scale = (float) sqlite3_column_double(stmt, 9);
      values.body_type = sqlite3_column_int(stmt, 10);
      values.interact_r = sqlite3_column_double(stmt, 11);
      values.interact_h = sqlite3_column_double(stmt, 12);
      values.x_offset = sqlite3_column_double(stmt, 13);
      values.y_offset = sqlite3_column_double(stmt, 14);
      auto name = sqlite3_column_text(stmt, 15);
      const char *s = (const char *) name;
      values.equip_type = std::string(reinterpret_cast< const char *> (s));
      auto race = sqlite3_column_text(stmt, 16);
      const char *g = (const char *) race;
      values.race = std::string(reinterpret_cast< const char *> (g));
      auto temp_type_name = sqlite3_column_text(stmt, 17);
      const char *h = (const char *) temp_type_name;
      values.temp_type_name = std::string(reinterpret_cast< const char *> (h));
      //std::cout << "data: " << name << std::endl;
    }
    return values;
  }

  std::vector<db::Unit_Data> Get_Unit_Subtypes(std::string race, std::string entity_class) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    std::string race_name = db::Append_Quotes(race);
    std::string type_name = db::Append_Quotes(entity_class);
    std::string text = race_name + " AND entity_class = " + type_name;// + " AND sprite_sheet = texture_packer";
    std::vector<db::Unit_Data> db_name;

    db::Unit_Data data;
    const unsigned char *name;
    const unsigned char *imgPath;
    sqlite3_stmt *stmt;
    char buf[300];

    const char *jj = "SELECT name, image_path FROM unit_data WHERE race = ";
    strcpy(buf, jj);
    strcat(buf, text.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));

      imgPath = sqlite3_column_text(stmt, 1);
      const char *d = (const char *) imgPath;
      std::string retpath = std::string(reinterpret_cast< const char *> (d));

      data = {retname, retpath};
      db_name.push_back(data);
    }
    db_name.shrink_to_fit();
    return db_name;
  }

  std::vector<int> Get_Race_Relationsips(std::string race) {// needs to search for  a specific row that I can input in the arguments
    std::vector<int> raceData;
    //check if the name exists??
    std::string raceInput = db::Append_Quotes(race);

    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT race_enum, rogue, human, fleshbeast, goblin, beast, demon, elf, zombie, skeleton, orc, dwarf, monster, eldritch, drow, nature FROM race_relationships WHERE race = ";
    strcpy(buf, jj);
    strcat(buf, raceInput.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      int raceID = sqlite3_column_int(stmt, 0);
      raceData.emplace_back(raceID);
      int rogue = sqlite3_column_int(stmt, 1);
      raceData.emplace_back(rogue);
      int human = sqlite3_column_int(stmt, 2);
      raceData.emplace_back(human);
      int fleshbeast = sqlite3_column_int(stmt, 3);
      raceData.emplace_back(fleshbeast);
      int goblin = sqlite3_column_int(stmt, 4);
      raceData.emplace_back(goblin);
      int beast = sqlite3_column_int(stmt, 5);
      raceData.emplace_back(beast);
      int demon = sqlite3_column_int(stmt, 6);
      raceData.emplace_back(demon);
      int elf = sqlite3_column_int(stmt, 7);
      raceData.emplace_back(elf);
      int zombie = sqlite3_column_int(stmt, 8);
      raceData.emplace_back(zombie);
      int skeleton = sqlite3_column_int(stmt, 9);
      raceData.emplace_back(skeleton);
      int orc = sqlite3_column_int(stmt, 10);
      raceData.emplace_back(orc);
      int dwarf = sqlite3_column_int(stmt, 11);
      raceData.emplace_back(dwarf);
      int monster = sqlite3_column_int(stmt, 12);
      raceData.emplace_back(monster);
      int eldtritch = sqlite3_column_int(stmt, 13);
      raceData.emplace_back(eldtritch);
      int drow = sqlite3_column_int(stmt, 14);
      raceData.emplace_back(drow);
      int nature = sqlite3_column_int(stmt, 15);
      raceData.emplace_back(nature);
      int neutral = sqlite3_column_int(stmt, 16);
      raceData.emplace_back(neutral);
    }

    raceData.shrink_to_fit();

    return raceData;
  }

  std::vector<std::string> Get_Tileset_Objects(std::string race, std::string type) {
    std::string race_name = db::Append_Quotes(race);
    std::string type_name = db::Append_Quotes(type);
    std::string text = race_name + " AND type = " + type_name + " AND tileset = 'forest'";
    std::vector<std::string> db_name;

    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];

    const char *jj = "SELECT name FROM buildings WHERE race = ";
    strcpy(buf, jj);
    strcat(buf, text.c_str());

    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);
    }
    db_name.shrink_to_fit();
    //sort
    std::sort(db_name.begin(), db_name.end(), [](const std::string &lhs, const std::string &rhs) {
      return lhs < rhs;
    });
    return db_name;
  }

  std::vector<std::string> Get_Names_Of_SubType(std::string race, std::string type, std::string subtype, std::string collider_type) {
    std::string race_name = db::Append_Quotes(race);
    std::string type_name = db::Append_Quotes(type);
    std::string class_name = db::Append_Quotes(subtype);
    std::string subtype_name = db::Append_Quotes(collider_type);
    std::string text = race_name + " AND type = " + type_name + " AND subtype = " + class_name + " AND collider_type = " + subtype_name;
    std::vector<std::string> db_name;

    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];

    const char *jj = "SELECT name FROM buildings WHERE race = ";
    strcpy(buf, jj);
    strcat(buf, text.c_str());

    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);
    }
    db_name.shrink_to_fit();
    //sort 
    std::sort(db_name.begin(), db_name.end(), [](const std::string &lhs, const std::string &rhs) {
      return lhs < rhs;
    });
    return db_name;
  }

  std::vector<std::string> Get_All_Names_Of_SubType(std::string &type) {
    std::string class_name = db::Append_Quotes(type);
    std::vector<std::string> db_name;

    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT name FROM buildings WHERE type = ";
    strcpy(buf, jj);
    strcat(buf, class_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);

    }
    db_name.shrink_to_fit();
    return db_name;
  }

  std::string Get_Tileset_Path(std::string tilesetName) {
    std::string tileset = db::Append_Quotes(tilesetName);
    std::string tilesetPath;

    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT path FROM tilesets WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, tileset.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      tilesetPath = std::string(reinterpret_cast< const char *> (s));
    }
    return tilesetPath;
  }

  std::vector<std::string> Get_All_Subtype_Of_Type(std::string &type) {
    std::string class_name = db::Append_Quotes(type);
    std::vector<std::string> db_name;

    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT subtype FROM building WHERE type = ";
    strcpy(buf, jj);
    strcat(buf, class_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);

    }
    db_name.shrink_to_fit();
    return db_name;
  }

  std::vector<std::string> Get_All_Types_Of_Race(std::string &type) {
    std::string class_name = db::Append_Quotes(type);
    std::vector<std::string> db_name;

    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT type FROM buildings WHERE race = ";
    strcpy(buf, jj);
    strcat(buf, class_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);

    }
    db_name.shrink_to_fit();
    return db_name;
  }

  std::vector<std::string> Get_All_Of_Races() {// needs to search for  a specific row that I can input in the arguments
    std::vector<std::string> db_name;
    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT race FROM race_relationships";
    strcpy(buf, jj);
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);
    }
    db_name.shrink_to_fit();
    return db_name;
  }

  std::vector<std::string> Get_All_Items_Of_Type(std::string &type) {
    std::string class_name = db::Append_Quotes(type);
    std::vector<std::string> db_name;

    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT type FROM weapon_types WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, class_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);
    }
    db_name.shrink_to_fit();
    return db_name;
  }

  std::vector<std::string> Get_All_Objects_Of_Type(std::string &type) {
    std::string class_name = db::Append_Quotes(type);
    std::vector<std::string> db_name;

    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT name FROM buildings WHERE type = ";
    strcpy(buf, jj);
    strcat(buf, class_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);

    }
    db_name.shrink_to_fit();
    return db_name;
  }

  std::vector<std::string> Get_All_Units_Of_Type(std::string &type) {
    std::string class_name = db::Append_Quotes(type);
    std::vector<std::string> db_name;

    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT name FROM unit_data WHERE type = ";
    strcpy(buf, jj);
    strcat(buf, class_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);

    }
    db_name.shrink_to_fit();
    return db_name;
  }

  std::vector<std::string> Get_All_Of_Type() {// needs to search for  a specific row that I can input in the arguments
    std::vector<std::string> db_name;
    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT object_type FROM unit_types";
    strcpy(buf, jj);
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);
    }
    db_name.shrink_to_fit();
    return db_name;
  }


  ////
  /// std::string entity_class MUST BE SINGLE QUOTES WRAPPED IN DOUBLE QUOTES OR ELSE IT THROWS A MEMORY READ EXCEPTION
  ///
  ///
  std::string Get_All_Of_Class(std::string entity_class) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    std::string class_name = db::Append_Quotes(entity_class);
    std::vector<std::string> db_name;
    const unsigned char *name;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT name FROM unit_data WHERE entity_class = ";
    strcpy(buf, jj);
    strcat(buf, class_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      name = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) name;
      std::string retname = std::string(reinterpret_cast< const char *> (s));
      db_name.push_back(retname);

    }
    //get random index for name
    int i = rand() % (db_name.size()) + 1;
    std::string output_name = db_name.at(i - 1);
    return output_name;
  }

  ////
  /// std::string entity_class MUST BE SINGLE QUOTES WRAPPED IN DOUBLE QUOTES OR ELSE IT THROWS A MEMORY READ EXCEPTION
  ///I should probably just fix that
  std::string Get_Sprite_Sheet(std::string &name) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    std::string unit_name = db::Append_Quotes(name);
    const unsigned char *sheet;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT sprite_sheet FROM unit_data WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, unit_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      sheet = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) sheet;
      unit_name = std::string(reinterpret_cast< const char *> (s));
    }
    return unit_name;
  }

  std::string Get_Building_Sprite_layout(std::string &name) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    std::string unit_name = db::Append_Quotes(name);
    std::string sprite_layout;
    const unsigned char *sheet;
    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT sprite_layout FROM buildings WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, unit_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      sheet = sqlite3_column_text(stmt, 0);
      const char *s = (const char *) sheet;
      sprite_layout = std::string(reinterpret_cast< const char *> (s));
    }
    return sprite_layout;
  }

  struct Building_Data {
    std::string collider_type = "none";
    float x_offset = 0.0f;
    float y_offset = 0.0f;
    float radius = 0.0f;
    std::string sprite_layout = "";
    std::string xml = "";
    std::string img = "";
  };

  std::string Get_String(const unsigned char *text) {
    const char *s;
    s = (const char *) text;
    if (s != NULL) {
      return std::string(reinterpret_cast< const char *> (s));
    }
    return "";
  }

  Building_Data Get_Building_Data(std::string &name) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    std::string unit_name = db::Append_Quotes(name);
    Building_Data data;
    const unsigned char *text;

    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT collider_type, radius, x_offset, y_offset, sprite_layout, xml, img FROM buildings WHERE name = ";
    strcpy(buf, jj);
    strcat(buf, unit_name.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      text = sqlite3_column_text(stmt, 0);
      data.collider_type = Get_String(text);

      data.radius = (float) sqlite3_column_double(stmt, 1);
      data.x_offset = (float) sqlite3_column_double(stmt, 2);
      data.y_offset = (float) sqlite3_column_double(stmt, 3);

      text = sqlite3_column_text(stmt, 4);
      data.sprite_layout = Get_String(text);

      text = sqlite3_column_text(stmt, 5);
      data.xml = Get_String(text);

      text = sqlite3_column_text(stmt, 6);
      data.img = Get_String(text);
    }
    return data;
  }
}
