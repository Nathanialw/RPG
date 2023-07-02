#include "item_components.h"
#include "entt/entt.hpp"
#include "components.h"
#include "items.h"

namespace equip_player_start {

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


    enum Role {
        archer,
        warrior,
        mage
    };

    struct Character {
        Armor_Type armorType;
        std::string equipType;
    };

    Character Create_Character (std::string gender, Role role) {
        std::string equipType;
        if (gender == "male") {
            equipType = "classes_male";
        }
        else {
            equipType = "classes_female";
        }

        Item_Component::Armor_Type armorType;

        switch (role) {
            case archer: armorType = Item_Component::Armor_Type::leather;
            case warrior: armorType = Item_Component::Armor_Type::mail;
            case mage: armorType = Item_Component::Armor_Type::cloth;
        }
        Character character = {armorType, equipType};

        return character;
    }

    entt::entity Create_Starter_Chest(Component::Position &position, Item_Component::Item_Type itemType, std::string &equip_type, Armor_Type armorType) {


        Item_Stats itemStats = Items::Generate_Item_Stats(rarity);
        Rarity rarity = Item_Component::Rarity::common;

        auto item_uID = World::zone.create();
        std::string itemName = Items::Create_Specific_Armor(item_uID, rarity, Item_Component::Item_Type::chest, armorType, equip_type);

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
            Character character =  Create_Character("female", Role::warrior);

            //create a weapon Component::Position& position, Component::Direction &direction
            equipment.equippedItems[Item_Type::mainhand] = Create_And_Equip_Weapon(position, equipment.type);
            equipment.equippedItems[Item_Type::offhand] = Create_And_Equip_Offhand(position, equipment.type);
            //create a chest
            equipment.equippedItems[Item_Type::chest] = Create_Starter_Chest(position, Item_Type::chest, character.equipType, character.armorType);
            //create a leggings
            //create hair
            //create kilt
//            equipment.equippedItems[Item_Type::clothes] = Items::Create_And_Equip_Armor(position, Item_Type::clothes, equipment.type);

            zone.emplace<Item_Component::Item_Equip>(unit);
        }
    }

//run when equipping or unequipping an item
    void Update_Equip_slots(entt::registry &zone) { //run funtion on item equip or unequip
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


/*
 * pants
 *      skirt   if female
 *      pants   if male
 *
 * chest
 *      tunic
 *
 * 2h           if warrior
 * ranged       if archer
 * staff        if mage
 *
 * */