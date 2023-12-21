#pragma once
#include "item_components.h"
#include "entt/entt.hpp"
#include "components.h"
#include "items.h"

namespace equip_unit {

  /*
   * creat a set of armour based on the chosen class
   *
   *
   * */

  entt::entity Create_And_Equip_Weapon(Component::Position &position, std::string &equip_type) {
    Rarity rarity = Items::Generate_Item_Rarity();
    Item_Stats itemStats = Items::Generate_Item_Stats(rarity);
    auto item_uID = World::zone.create();
    std::string itemName = Items::Create_Weapon(item_uID, rarity, equip_type);
    if (itemName == "none") {
      World::zone.destroy(item_uID);
      Utilities::Log("Create_And_Equip_Armor() no item in db, no item has been created");
      return Item_Component::emptyEquipSlot;
    }
    Items::Create_Item1(item_uID, position, itemName, itemStats);
    return item_uID;
  }


  entt::entity Create_And_Equip_Offhand(Component::Position &position, std::string &equip_type) {
    Rarity rarity = Items::Generate_Item_Rarity();
    Item_Stats itemStats = Items::Generate_Item_Stats(rarity);
    auto item_uID = World::zone.create();
    std::string itemName = Items::Create_Offhand(item_uID, rarity, equip_type);
    if (itemName == "none") {
      World::zone.destroy(item_uID);
      Utilities::Log("Create_And_Equip_Armor() no item in db, no item has been created");
      return Item_Component::emptyEquipSlot;
    }
    Items::Create_Item1(item_uID, position, itemName, itemStats);
    return item_uID;
  }

  entt::entity Create_And_Equip_Armor(Component::Position &position, Item_Component::Item_Type itemType, Item_Component::Item &equip_type) {
    Rarity rarity = Items::Generate_Item_Rarity();
    Item_Stats itemStats = Items::Generate_Item_Stats(rarity);
    Armor_Type armorType = Items::Generate_Armor_Type();
    auto item_uID = World::zone.create();
    std::string itemName = Items::Create_Specific_Armor(item_uID, rarity, itemType, armorType, equip_type.name);
    if (itemName == "none") {
      World::zone.destroy(item_uID);
      Utilities::Log("Create_And_Equip_Armor() no item in db, no item has been created");
      return Item_Component::emptyEquipSlot;
    } else {
      Items::Create_Item1(item_uID, position, itemName, itemStats);
      return item_uID;
    }
  }


  void Equip_Units(entt::registry &zone) {
    auto view = zone.view<Item_Component::Equipment, Component::Position>();
    for (auto unit: view) {
      auto &equipment = view.get<Item_Component::Equipment>(unit);
      auto &position = view.get<Component::Position>(unit);

      //create a weapon Component::Position& position, Component::Direction &direction
      equipment.equippedItems[Item_Type::mainhand] = Create_And_Equip_Weapon(position, equipment.type);
      equipment.equippedItems[Item_Type::offhand] = Create_And_Equip_Offhand(position, equipment.type);
      //create a chest
      equipment.equippedItems[Item_Type::legs] = Create_And_Equip_Armor(position, Item_Type::legs, equipment.type);
      //create a leggings
      equipment.equippedItems[Item_Type::chest] = Create_And_Equip_Armor(position, Item_Type::chest, equipment.type);
      //create hair
      equipment.equippedItems[Item_Type::hair] = Create_And_Equip_Armor(position, Item_Type::hair, equipment.type);
      //create kilt
      //            equipment.equippedItems[Item_Type::clothes] = Items::Create_And_Equip_Armor(position, Item_Type::clothes, equipment.type);

      zone.emplace_or_replace<Item_Component::Item_Equip>(unit);
    }
  }

  //run when equipping or unequipping an item
  void Update_Equip_slots(entt::registry &zone) { //run function on item equip or unequip
    auto view = zone.view<Rendering_Components::Sprite_Sheet_Info, Item_Component::Item_Equip, Item_Component::Equipment, Rendering_Components::Equipment_Sprites>();
    for (auto entity: view) {
      auto &sheetData = view.get<Rendering_Components::Sprite_Sheet_Info>(entity);
      auto &equipment = view.get<Item_Component::Equipment>(entity);
      auto &equipmentSprites = view.get<Rendering_Components::Equipment_Sprites>(entity);

      //iterate through each equip slot
      for (auto &item: equipment.equippedItems) {
	if (item.second != Item_Component::emptyEquipSlot) {

	  //get the item at the item type index
	  auto &weaponSheet = zone.get<Rendering_Components::Sprite_Sheet_Info>(item.second);
	  if (weaponSheet.sheetData) {
	    equipmentSprites.sheet[(int) item.first].ItemSheetData = weaponSheet.sheetData;
	    equipmentSprites.sheet[(int) item.first].name = weaponSheet.sheet_name;
	    equipmentSprites.sheet[(int) item.first].itemID = item.second;
	  }
	} else if (item.second == Item_Component::emptyEquipSlot) {
	  equipmentSprites.sheet[(int) item.first].ItemSheetData = NULL;
	  equipmentSprites.sheet[(int) item.first].name = "empty";
	  equipmentSprites.sheet[(int) item.first].itemID = emptyEquipSlot;
	}
      }
    }
  }
}

