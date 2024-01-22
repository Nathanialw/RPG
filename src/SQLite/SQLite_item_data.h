#pragma once
#include "db.h"
#include "item_components.h"
#include "vector"
#include <SDL2/SDL.h>
#include <iostream>
#include <string.h>

namespace SQLite_Item_Data {

  // read in all items and emplace each unit type into the map and fill it up
  //  holds a map for each unit type >>>  holds a vector for each item type for the unit >>> the vector is a list of strings of the name of each item in the db
  std::unordered_map<Item_Component::Unit_Equip_Type, std::unordered_map<Item_Component::Item_Type, std::vector<Item_Component::Item>>> Items;
  //    std::unordered_map<Item_Component::Unit_Equip_Type, std::unordered_map<Item_Component::Item_Type, std::unordered_map<std::string, Item_Component::Item>>>Items;
  //    std::array<std::array<std::vector<std::string>, (int)Item_Component::Item_Type::size>, (int)Item_Component::Unit_Equip_Type::size>Items;

  void Load_Item_Names() {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    //        std::string unit_name = db::Append_Quotes(unit_type);
    Item_Component::Item item;

    sqlite3_stmt *stmt;
    char buf[1000];
    const char *jj = "SELECT slot, type, equip_type, face, body, has_texture, item_type FROM weapon_types";
    strcpy(buf, jj);
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      //            get the index of the enum of Item_Components::Item_Type
      auto itemType = sqlite3_column_text(stmt, 0);//0 only increments up when calling more than one column
      std::string g = db::Convert_Char("slot", itemType);
      Item_Component::Item_Type item_type = Item_Component::Get_Item_Type(g);

      //get the name of the item as a sting
      auto type = sqlite3_column_text(stmt, 1);//0 only increments up when calling more than one column
      item.name = db::Convert_Char("type", type);

      // get the RTP_male, classes_female etc strings
      auto db_equip_type = sqlite3_column_text(stmt, 2);//0 only increments up when calling more than one column
      std::string equip_type = db::Convert_Char("equip_type", db_equip_type);

      // get the face etc strings
      auto db_face = sqlite3_column_text(stmt, 3);//0 only increments up when calling more than one column
      item.face_pngPath = db::Convert_Char("face", db_face);

      // get the body etc strings
      auto db_body = sqlite3_column_text(stmt, 4);//0 only increments up when calling more than one column
      item.body_pngPath = db::Convert_Char("body", db_body);

      // get the body etc strings
      auto db_has_texture = sqlite3_column_int(stmt, 5);//0 only increments up when calling more than one column
      db::Get_bool("body", db_has_texture) ? item.hasTexture = true : item.hasTexture = false;

      //item type
      auto db_item_type = sqlite3_column_text(stmt, 6);//0 only increments up when calling more than one column
      item.weapon_type = db::Convert_Char("item_type", db_item_type);

      Item_Component::Unit_Equip_Type unit_equip_type = Item_Component::Get_Unit_Equip_Type(equip_type);
      Items[unit_equip_type][item_type].emplace_back(item);
    }
    //        Utilities::Log(Items.size());
    //        Utilities::Log("Loading items success!");
  }
  Item_Component::Item Load_Specific_Item(std::string itemName) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    //        std::string unit_name = db::Append_Quotes(unit_type);
    std::string text = db::Append_Quotes(itemName);
    Item_Component::Item item;

    sqlite3_stmt *stmt;
    char buf[1000];
    const char *jj = "SELECT slot, type, equip_type, face, body, icon_name, has_texture, item_type FROM weapon_types WHERE type = ";
    strcpy(buf, jj);
    strcat(buf, text.c_str());

    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      //            get the index of the enum of Item_Components::Item_Type
      auto itemType = sqlite3_column_text(stmt, 0);//0 only increments up when calling more than one column
      std::string g = db::Convert_Char("slot", itemType);
      Item_Component::Item_Type item_type = Item_Component::Get_Item_Type(g);

      //get the name of the item as a sting
      auto type = sqlite3_column_text(stmt, 1);//0 only increments up when calling more than one column
      item.name = db::Convert_Char("type", type);

      // get the RTP_male, classes_female etc strings
      auto db_equip_type = sqlite3_column_text(stmt, 2);//0 only increments up when calling more than one column
      std::string equip_type = db::Convert_Char("equip_type", db_equip_type);
      item.equip_type = Item_Component::Get_Unit_Equip_Type(equip_type);

      // get the face etc strings
      auto db_face = sqlite3_column_text(stmt, 3);//0 only increments up when calling more than one column
      item.face_pngPath = db::Convert_Char("face", db_face);

      // get the body etc strings
      auto db_body = sqlite3_column_text(stmt, 4);//0 only increments up when calling more than one column
      item.body_pngPath = db::Convert_Char("body", db_body);

      //icon name
      auto db_icon = sqlite3_column_text(stmt, 5);//0 only increments up when calling more than one column
      item.icon_name = db::Convert_Char("icon_name", db_icon);

      //has texture
      auto db_has_texture = sqlite3_column_int(stmt, 6);//0 only increments up when calling more than one column
      db::Get_bool("body", db_has_texture) ? item.hasTexture = true : item.hasTexture = false;

      //item type
      auto db_item_type = sqlite3_column_text(stmt, 7);//0 only increments up when calling more than one column
      item.weapon_type = db::Convert_Char("item_type", db_item_type);

      return item;
    }
    return item;
    //        Utilities::Log(Items.size());
    //        Utilities::Log("Loading items success!");
  }

  std::vector<Item_Component::Item> Load_Item(std::string slotType, std::string equipType) {// needs to search for  a specific row that I can input in the arguments
    //check if the name exists??
    std::vector<Item_Component::Item> items;
    Item_Component::Item item;
    slotType = db::Append_Quotes(slotType);
    equipType = db::Append_Quotes(equipType);

    std::string statement = slotType + " AND equip_type = " + equipType;
    sqlite3_stmt *stmt;
    char buf[1000];
    const char *jj = "SELECT slot, type, face, body, icon_name, has_texture, item_type FROM weapon_types WHERE slot = ";
    strcpy(buf, jj);
    strcat(buf, statement.c_str());

    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      //            get the index of the enum of Item_Components::Item_Type
      auto itemType = sqlite3_column_text(stmt, 0);//0 only increments up when calling more than one column
      std::string g = db::Convert_Char("slot", itemType);
      Item_Component::Item_Type item_type = Item_Component::Get_Item_Type(g);

      //get the name of the item as a sting
      auto type = sqlite3_column_text(stmt, 1);//0 only increments up when calling more than one column
      item.name = db::Convert_Char("type", type);

      // get the RTP_male, classes_female etc strings
      //      auto db_equip_type = sqlite3_column_text(stmt, 2);//0 only increments up when calling more than one column
      //      std::string equip_type = db::Convert_Char("equip_type", db_equip_type);
      //      item.equip_type = Item_Component::Get_Unit_Equip_Type(equip_type);

      // get the face etc strings
      auto db_face = sqlite3_column_text(stmt, 2);//0 only increments up when calling more than one column
      item.face_pngPath = db::Convert_Char("face", db_face);

      // get the body etc strings
      auto db_body = sqlite3_column_text(stmt, 3);//0 only increments up when calling more than one column
      item.body_pngPath = db::Convert_Char("body", db_body);

      //icon name
      auto db_icon = sqlite3_column_text(stmt, 4);//0 only increments up when calling more than one column
      item.icon_name = db::Convert_Char("icon_name", db_icon);

      //has texture
      auto db_has_texture = sqlite3_column_int(stmt, 5);//0 only increments up when calling more than one column
      db::Get_bool("body", db_has_texture) ? item.hasTexture = true : item.hasTexture = false;

      //item type
      auto db_item_type = sqlite3_column_text(stmt, 6);//0 only increments up when calling more than one column
      item.weapon_type = db::Convert_Char("item_type", db_item_type);

      items.emplace_back(item);
    }
    Utilities::Log("items count: " + std::to_string(items.size()));
    return items;
  }
}// namespace SQLite_Item_Data