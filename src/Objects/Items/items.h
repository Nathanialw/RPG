#pragma once

#include "Graphics/XML_Parsers/texture_packer_item.h"
#include "Input/camera.h"
#include "Input/mouse_control.h"
#include "Maps/World/world.h"
#include "SQLite_item_data.h"
#include "SQLite_spritesheets.h"
#include "building_components.h"
#include "components.h"
#include "graphics.h"
#include "icons.h"
#include "item_components.h"
#include "quad_tree.h"
#include "utilities.h"
#include <vector>
#include "colors.h"

using namespace Item_Component;

namespace Items {
    namespace {
        bool showGroundItems = false;
    }

    int i = 0;

    Item_Component::Weapon_Type Generate_Weapon_Type() {
        int randType = rand() % 4 + 1;
        switch (randType) {
            case 1:
                return Item_Component::Weapon_Type::sword;
            case 2:
                return Item_Component::Weapon_Type::mace;
            case 3:
                return Item_Component::Weapon_Type::axe;
            case 4:
                return Item_Component::Weapon_Type::spear;
        }
        Utilities::Log("Generate_Weapon_Type() fallthrough error");
        //defult return if it finds nothing
        return Weapon_Type::sword;
    }

    Item_Component::Weapon_Type Generate_Offhand_Type() {
        int randType = rand() % 4 + 1;
        switch (randType) {
            case 1:
                return Item_Component::Weapon_Type::sword;
            case 2:
                return Item_Component::Weapon_Type::mace;
            case 3:
                return Item_Component::Weapon_Type::axe;
            case 4:
                return Item_Component::Weapon_Type::spear;
        }
        Utilities::Log("Generate_Weapon_Type() fallthrough error");
        //defult return if it finds nothing
        return Weapon_Type::sword;
    }

    Item_Component::Item_Type Generate_Item_Type() {
        int randType = rand() % 5 + 1;

        switch (randType) {
            case 1:
                return Item_Type::helm;
            case 2:
                return Item_Type::chest;
            case 3:
                return Item_Type::gloves;
            case 4:
                return Item_Type::legs;
            case 5:
                return Item_Type::boots;
        }
        Utilities::Log("Generate_Item_Type() fallthrough error");
        //defult return if it finds nothing
        return Item_Type::helm;
    }

    Item_Component::Weapon_Material Generate_Weapon_Material() {
        int randType = rand() % 3 + 1;

        switch (randType) {
            case 1:
                return Weapon_Material::copper;
            case 2:
                return Weapon_Material::bronze;
            case 3:
                return Weapon_Material::iron;
        }
        Utilities::Log("Generate_Weapon_Material() fallthrough error");
        //defult return if it finds nothing
        return Weapon_Material::copper;
    }

    Item_Component::Armor_Type Generate_Armor_Type() {
        int randType = rand() % 5 + 1;

        switch (randType) {
            case 1:
                return Armor_Type::cloth;
            case 2:
                return Armor_Type::padded;
            case 3:
                return Armor_Type::leather;
            case 4:
                return Armor_Type::mail;
            case 5:
                return Armor_Type::plate;
        }
        Utilities::Log("Generate_Armor_Type() fallthrough error");
        //defult return if it finds nothing
        return Armor_Type::cloth;
    }

    std::string Create_Weapon(entt::registry &zone, entt::entity &item, Rarity &rarity, Item_Component::Unit_Equip_Type &equip_type, Item_Component::Item item_name, SDL_Color color) {
        auto material = Generate_Weapon_Material();
        Weapon_Type weaponType = Get_Weapon_Type(item_name.weapon_type);
        if (weaponType == Weapon_Type::SIZE) { return "none"; };

        std::string materialType = weaponMaterialName[material];
        std::string weapon = weaponTypeName[weaponType];
        std::string weaponName = materialType + " " + weapon;
        std::string slot = "mainhand";

        auto &type = zone.emplace_or_replace<Item_Type>(item, Item_Type::mainhand);
        zone.emplace_or_replace<Rarity>(item, rarity);

        Texture_Packer_Item::Get_Item_Portrait_Texture(item_name.name, ("assets/" + item_name.face_pngPath).c_str());
        Texture_Packer_Item::Get_Item_Body_Texture(item_name.name, ("assets/" + item_name.body_pngPath).c_str());
        zone.emplace_or_replace<Rendering_Components::Portrait>(item, Texture_Packer_Item::Item_Portaits[item_name.name], color);
        zone.emplace_or_replace<Rendering_Components::Body>(item, Texture_Packer_Item::Item_Body[item_name.name], color);

        ///create the weapon from the db
        ///add to std::map
        ///save the string of the name
        ///provide lookup string when the player picks it up

        auto equippedSheetData = Texture_Packer_Item::TexturePacker_Import_Item(slot, equip_type, item_name.name);
        zone.emplace_or_replace<Item_Component::Weapon_Type>(item, weaponType);

        if (!equippedSheetData.itemData) {
            return "none";
        }

        auto &sheetData = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(item);
        sheetData.type = "RPG_Tools";
        sheetData.sheetData = equippedSheetData.itemData;
        sheetData.sheet_name = equippedSheetData.index;

        Icons::Icon_Clip sprite = Icons::iconClipRects[item_name.icon_name];

        auto &icon = zone.emplace_or_replace<Component::Icon>(item, Texture::emptyBagIcon, Texture::icons, rarityBorder[rarity], Texture::bagSlotBorder);
        icon.clipSprite = sprite.clipRect;
        icon.clipIcon = {0, 0, 256, 256};
        icon.renderRectSize = {64.0f, 64.0f};
        icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};

        zone.emplace_or_replace<Weapon_Damage>(item, 1, 10);
        return equippedSheetData.index;
    }

    std::string Create_Offhand(entt::registry &zone, entt::entity &item, Rarity &rarity, Item_Component::Unit_Equip_Type &equip_type, Item_Component::Item &item_name, SDL_Color &color) {
        auto material = Generate_Weapon_Material();
        auto offhandType = Item_Component::Offhand_Type::shield;

        std::string materialType = weaponMaterialName[material];
        std::string offhand = offhandTypeName[offhandType];
        std::string offhandName = materialType + " " + offhand;
        std::string slot = "offhand";

        auto &type = zone.emplace_or_replace<Item_Type>(item, Item_Type::offhand);
        zone.emplace_or_replace<Rarity>(item, rarity);

        ///create the weapon from the db
        ///add to std::map
        ///save the string of the name
        ///provide lookup string when the player picks it up

        auto equippedSheetData = Texture_Packer_Item::TexturePacker_Import_Item(slot, equip_type, item_name.name);

        Texture_Packer_Item::Get_Item_Portrait_Texture(item_name.name, ("assets/" + item_name.face_pngPath).c_str());
        Texture_Packer_Item::Get_Item_Body_Texture(item_name.name, ("assets/" + item_name.body_pngPath).c_str());
        zone.emplace_or_replace<Rendering_Components::Portrait>(item, Texture_Packer_Item::Item_Portaits[item_name.name], color);
        zone.emplace_or_replace<Rendering_Components::Body>(item, Texture_Packer_Item::Item_Body[item_name.name], color);

        if (!equippedSheetData.itemData) {
            return "none";
        }

        auto &sheetData = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(item);
        sheetData.type = "RPG_Tools";
        sheetData.sheetData = equippedSheetData.itemData;
        sheetData.sheet_name = equippedSheetData.index;

        Icons::Icon_Clip sprite = Icons::iconClipRects[item_name.icon_name];

        auto &icon = zone.emplace_or_replace<Component::Icon>(item, Texture::emptyBagIcon, Texture::icons, rarityBorder[rarity], Texture::bagSlotBorder);
        icon.clipSprite = sprite.clipRect;
        icon.clipIcon = {0, 0, 256, 256};
        icon.renderRectSize = {64.0f, 64.0f};
        icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};

        zone.emplace_or_replace<Weapon_Damage>(item, 1, 10);
        return equippedSheetData.index;
    }

    std::string Create_Specific_Armor(entt::registry &zone, entt::entity &item, Rarity &rarity, Item_Type itemType, Armor_Type armorType, Item_Component::Unit_Equip_Type &equip_type, Item_Component::Item &item_name, SDL_Color &color) {
        //        Armor_Type armorType = Generate_Armor_Type();
        std::string type = ItemTypeName[itemType];
        std::string armor = ArmorTypeName[armorType];
        std::string itemName = armor + " " + type;//ei. "Copper Pants" not implemented yet
        zone.emplace_or_replace<Item_Type>(item, itemType);
        zone.emplace_or_replace<Rarity>(item, rarity);

        Texture_Packer_Item::Item_Data_And_Index equippedSheetData;
        Texture_Packer_Item::Get_Item_Portrait_Texture(item_name.name, ("assets/" + item_name.face_pngPath).c_str());
        Texture_Packer_Item::Get_Item_Body_Texture(item_name.name, ("assets/" + item_name.body_pngPath).c_str());
        zone.emplace_or_replace<Rendering_Components::Portrait>(item, Texture_Packer_Item::Item_Portaits[item_name.name], color);
        zone.emplace_or_replace<Rendering_Components::Body>(item, Texture_Packer_Item::Item_Body[item_name.name], color);

        if (item_name.hasTexture) {
            equippedSheetData = Texture_Packer_Item::TexturePacker_Import_Item(type, equip_type, item_name.name);
            if (!equippedSheetData.itemData) {
                return "none";
            }
            auto &sheetData = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(item);
            sheetData.color = color;
            sheetData.sheet_name = equippedSheetData.index;
            sheetData.type = "RPG_Tools";
            sheetData.sheetData = equippedSheetData.itemData;
        } else {
            equippedSheetData.index = item_name.name;

            auto &sheetData = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(item);
            sheetData.color = color;
            sheetData.sheet_name = equippedSheetData.index;
            sheetData.type = "RPG_Tools";
            sheetData.sheetData = nullptr;
        }


        //look up clip from xml save to icon.clipSprite
        //save whole sprite sheet of icons to *iconTexture
        //    }
        Icons::Icon_Clip sprite = Icons::iconClipRects[item_name.icon_name];

        auto &icon = zone.emplace_or_replace<Component::Icon>(item, Texture::emptyBagIcon, Texture::icons, rarityBorder[rarity], Texture::bagSlotBorder);
        icon.clipSprite = sprite.clipRect;
        icon.clipIcon = {0, 0, 256, 256};
        icon.renderRectSize = {64.0f, 64.0f};
        icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};
        return equippedSheetData.index;
    }

    void Create_Item(entt::registry &zone, entt::entity &item, Component::Position &position, const std::string &name, Item_Stats &itemStats, Component::Direction &direction) {
        float scale = 0.7f;

        zone.emplace_or_replace<Component::Scale>(item, scale);
        zone.emplace_or_replace<Action_Component::Action>(item, Action_Component::dead);
        zone.emplace_or_replace<Component::Direction>(item, direction);
        zone.emplace_or_replace<Name>(item, name);
        zone.emplace_or_replace<Component::Entity_Type>(item, Component::Entity_Type::item);
        auto &stats = zone.emplace_or_replace<Item_Stats>(item);
        stats = itemStats;
        zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(item, 90.0f, 130.0f);
        zone.emplace_or_replace<Component::Position>(item, position.x, position.y);
    }

    statValue Get_Random_Stat() {
        int randStat = rand() % 6 + 1;
        int randStatValue = rand() % 5 + 1;
        switch (randStat) {
            case 1:
                return {Stat::health, randStatValue};
            case 2:
                return {Stat::damage, randStatValue};
            case 3:
                return {Stat::spellDamage, randStatValue};
            case 4:
                return {Stat::armor, randStatValue};
            case 5:
                return {Stat::piety, randStatValue};
            case 6:
                return {Stat::attackSpeed, randStatValue};
        }
        Utilities::Log("Get_Random_Stat() fallthrough error");
        return {Stat::health, randStatValue};
    }

    Rarity Generate_Item_Rarity() {
        int randType = rand() % 100 + 1;
        if (randType >= 35) {
            return Rarity::common;
        } else if (randType >= 15) {
            return Rarity::magic;
        } else if (randType >= 5) {
            return Rarity::rare;
        } else if (randType >= 0) {
            return Rarity::unique;
        }
        Utilities::Log("Generate_Item_Rarity() fallthrough error");
        return Rarity::common;
    }

    Item_Stats Generate_Item_Stats(Rarity &rarity) {
        Item_Stats itemStats;
        int numMods = 0;
        statValue randomStat;
        if (rarity == Rarity::common) {
            numMods = rand() % 2 + 1;
        } else if (rarity == Rarity::magic) {
            numMods = rand() % 4 + 2;
        } else if (rarity == Rarity::rare) {
            numMods = rand() % 6 + 3;
        } else if (rarity == Rarity::unique) {
            numMods = rand() % 8 + 4;
        }

        for (int index = 0; index < numMods; index++) {
            randomStat = Get_Random_Stat();
            if (randomStat.name == Stat::attackSpeed) { randomStat.value = randomStat.value * -10; };
            if (itemStats.stats.contains(randomStat.name)) {
                itemStats.stats[randomStat.name] += randomStat.value;
            } else {
                itemStats.stats.emplace(randomStat.name, randomStat.value);
            }
        }
        return itemStats;
    }

    std::string Choose_Item(entt::entity item, Rarity rarity) {
        int itemDrop = rand() % 10 + 1;
        if (itemDrop < 10) {
            //            return Create_Armor(item, rarity);
        } else {
            //            return Create_Weapon(item, rarity);
        }
        //        std::cout << "Choose_Item() fallthrough error, failed to select and item type, returned default" << std::endl;
        return "Create_Armor(item, rarity)";
    }

    void Drop_Item(entt::registry &zone, entt::entity &item_ID, Component::Position &itemPosition, Component::Direction &direction) {
        auto &sheetData = zone.get<Rendering_Components::Sprite_Sheet_Info>(item_ID);
        auto &clipRect = sheetData.sheetData->at(sheetData.sheet_name).frameList.at(sheetData.sheetData->at(sheetData.sheet_name).actionFrameData.at(Action_Component::dead).startFrame + (int) direction);
        SDL_FRect frec = Utilities::SDL_Rect_To_SDL_FRect(clipRect.clip);
        SDL_FRect rec = Utilities::Centre_Rect_On_Position(frec, itemPosition.x, itemPosition.y);

        zone.emplace_or_replace<Item_Component::Ground_Item>(item_ID, rec);
        zone.emplace_or_replace<Component::Interaction_Rect>(item_ID, rec.x, rec.y, (float) clipRect.clip.w, (float) clipRect.clip.h);
        zone.emplace_or_replace<Item_Component::Update_Ground_Item>(item_ID);
        zone.emplace_or_replace<Component::Radius>(item_ID, 10.0f);

        if (zone.any_of<Component::In_Object_Tree>(item_ID)) {
            Utilities::Log("item was still has In_Object_Tree");
            zone.remove<Component::Remove_From_Object_Tree>(item_ID);
            zone.remove<Component::In_Object_Tree>(item_ID);
        }
    }

    struct Item_Generation {
        bool created = false;
        entt::entity item;
    };

    Item_Generation Generate_Item(entt::registry &zone, Item_Component::Unit_Equip_Type equip_type) {
        Rarity rarity = Generate_Item_Rarity();
        Item_Stats itemStats = Generate_Item_Stats(rarity);

        Item_Component::Item item_name;
        SDL_Color color = Color::white;

        //    int type = rand() % 3 + 0;
        //    Utilities::Log(type);
        //    switch (type) {
        //      case 0: {
        //        Item_Component::Item_Type itemType = Item_Type::helm;
        //        Armor_Type armorType = Items::Generate_Armor_Type();
        //        equip_type == Unit_Equip_Type::classes_male ? item_name = SQLite_Item_Data::Load_Specific_Item("Male_Knight_Head") : item_name = SQLite_Item_Data::Load_Specific_Item("Female_Knight_Head");
        //        itemName = Create_Specific_Armor(zone, item_ID, rarity, itemType, armorType, equip_type, item_name, color);
        //        break;
        //      }
        //      case 1: {
        //        equip_type == Unit_Equip_Type::classes_male ? item_name = SQLite_Item_Data::Load_Specific_Item("Male_Battleguard_Sword") : item_name = SQLite_Item_Data::Load_Specific_Item("Female_Battleguard_Sword");
        //        itemName = Create_Weapon(zone, item_ID, rarity, equip_type, item_name, color);
        //        break;
        //      }
        //      case 2: {
        //        equip_type == Unit_Equip_Type::classes_male ? item_name = SQLite_Item_Data::Load_Specific_Item("Male_Footman_Shield") : item_name = SQLite_Item_Data::Load_Specific_Item("Female_Footman_Shield");
        //        itemName = Create_Offhand(zone, item_ID, rarity, equip_type, item_name, color);
        //        break;
        //      }
        //    }

        Item_Type itemType = Item_Type::hair;
        while (itemType == Item_Type::hair || itemType == Item_Type::facialHair || itemType == Item_Type::ring0 || itemType == Item_Type::ring1) {
            itemType = Item_Type(rand() % (int) Item_Type::SIZE + 0);
        }

        std::string item_type = Item_Component::Get_Item_Type_String(itemType);
        std::vector<Item_Component::Item> items = SQLite_Item_Data::Load_Item(item_type, Get_Unit_Equip_Type_String(equip_type));
        if (items.empty()) {
            Utilities::Log("no item found in db: " + item_type + " " + Get_Unit_Equip_Type_String(equip_type));
            return {false};
        }
        int type = rand() % items.size() + 0;
        item_name = items[type];

        Armor_Type armorType = Items::Generate_Armor_Type();
        auto item_ID = zone.create();
        std::string itemName = "";
        if (itemType == Item_Type::mainhand) {
            itemName = Create_Weapon(zone, item_ID, rarity, equip_type, item_name, color);
        } else {
            itemName = Create_Specific_Armor(zone, item_ID, rarity, itemType, armorType, equip_type, item_name, color);
        }
        if (itemName == "none") {
            zone.destroy(item_ID);
            Utilities::Log("no drop");
            return {false};
        }
        //	int item = rand() % 100 + 1;
        //	if (item <= 10) { Create_Item(item, position, rarity, "sword", Item_Type::weapon, Weapon_Type::sword, Graphics::longsword_default, Graphics::longsword_default_icon, itemStats); }
        //	else if (item <= 12) { Create_Item(item, position, rarity, "padded armour", Item_Type::chest, Armor_Type::cloth, Graphics::armorSpriteSheet, Graphics::armorSpriteSheet, itemStats); }
        Component::Position position = {0.0f, 0.0f};
        Component::Direction direction = Component::Direction::W;
        Create_Item(zone, item_ID, position, itemName, itemStats, direction);
        return {true, item_ID};
    }

    void Create_And_Drop_Item(entt::registry &zone, Component::Position &position, Component::Direction direction, Item_Component::Unit_Equip_Type equip_type) {
        Rarity rarity = Generate_Item_Rarity();
        Item_Stats itemStats = Generate_Item_Stats(rarity);
        auto item_ID = zone.create();

        Item_Component::Item item_name;
        SDL_Color color = Color::white;
        std::string itemName;

        int type = rand() % 3;
        Utilities::Log(type);


        //instead of random, use a loot table saved in the DB
        switch (type) {
            case 0: {
                Item_Component::Item_Type itemType = Item_Type::helm;
                item_name = SQLite_Item_Data::Items[equip_type][itemType][rand() % SQLite_Item_Data::Items[equip_type][itemType].size()];
                Armor_Type armorType = Items::Generate_Armor_Type();
                itemName = Create_Specific_Armor(zone, item_ID, rarity, itemType, armorType, equip_type, item_name, color);
                break;
            }
            case 1: {
                Item_Component::Item_Type itemType = Item_Type::mainhand;
                item_name = SQLite_Item_Data::Items[equip_type][itemType][rand() % SQLite_Item_Data::Items[equip_type][itemType].size()];
                itemName = Create_Weapon(zone, item_ID, rarity, equip_type, item_name, color);
                break;
            }
            case 2: {
                Item_Component::Item_Type itemType = Item_Type::offhand;
                item_name = SQLite_Item_Data::Items[equip_type][itemType][rand() % SQLite_Item_Data::Items[equip_type][itemType].size()];
                itemName = Create_Offhand(zone, item_ID, rarity, equip_type, item_name, color);
                break;
            }
        }

        if (itemName == "none") {
            zone.destroy(item_ID);
            Utilities::Log("no drop");
            return;
        }
        //	int item = rand() % 100 + 1;
        //	if (item <= 10) { Create_Item(item, position, rarity, "sword", Item_Type::weapon, Weapon_Type::sword, Graphics::longsword_default, Graphics::longsword_default_icon, itemStats); }
        //	else if (item <= 12) { Create_Item(item, position, rarity, "padded armour", Item_Type::chest, Armor_Type::cloth, Graphics::armorSpriteSheet, Graphics::armorSpriteSheet, itemStats); }
        Create_Item(zone, item_ID, position, itemName, itemStats, direction);
        Drop_Item(zone, item_ID, position, direction);
    }

    void Create_Item1(entt::registry &zone, entt::entity &item, Component::Position &position, const std::string &name, Item_Stats &itemStats) {
        float scale = 0.7f;
        zone.emplace_or_replace<Component::Scale>(item, scale);
        zone.emplace_or_replace<Action_Component::Action>(item, Action_Component::isStatic);
        zone.emplace_or_replace<Component::Entity_Type>(item, Component::Entity_Type::item);
        zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(item, 100.0f, 100.0f);

        zone.emplace_or_replace<Item_Stats>(item) = itemStats;
        zone.emplace_or_replace<Name>(item, name);
        zone.emplace_or_replace<Component::Position>(item, position.x, position.y);
    }

    entt::entity Create_And_Equip_Weapon(entt::registry &zone, int &state, Component::Position &position, Item_Component::Unit_Equip_Type &equip_type, Item_Component::Item item_name, SDL_Color color) {
        Rarity rarity = Generate_Item_Rarity();
        Item_Stats itemStats = Generate_Item_Stats(rarity);
        auto item_ID = zone.create();
        std::string itemName = Create_Weapon(zone, item_ID, rarity, equip_type, item_name, color);
        if (itemName == "none") {
            zone.destroy(item_ID);
            Utilities::Log("Create_And_Equip_Armor() no item in db, no item has been created");
            return Item_Component::emptyEquipSlot[state];
        }
        Create_Item1(zone, item_ID, position, itemName, itemStats);
        return item_ID;
    }

    entt::entity Create_And_Equip_Offhand(entt::registry &zone, int &state, Component::Position &position, Item_Component::Unit_Equip_Type &equip_type, Item_Component::Item item_name) {
        Rarity rarity = Generate_Item_Rarity();
        Item_Stats itemStats = Generate_Item_Stats(rarity);
        SDL_Color color;
        auto item_ID = zone.create();
        std::string itemName = Create_Offhand(zone, item_ID, rarity, equip_type, item_name, color);
        if (itemName == "none") {
            zone.destroy(item_ID);
            Utilities::Log("Create_And_Equip_Armor() no item in db, no item has been created");
            return Item_Component::emptyEquipSlot[state];
        }
        Create_Item1(zone, item_ID, position, itemName, itemStats);
        return item_ID;
    }

    entt::entity Create_And_Equip_Armor(entt::registry &zone, int &state, Component::Position &position, Item_Component::Item_Type itemType, Item_Component::Unit_Equip_Type &equip_type, Item_Component::Item item_name, SDL_Color color) {
        Rarity rarity = Generate_Item_Rarity();
        Item_Stats itemStats = Generate_Item_Stats(rarity);
        Armor_Type armorType = Items::Generate_Armor_Type();

        auto item_ID = zone.create();
        std::string itemName = Create_Specific_Armor(zone, item_ID, rarity, itemType, armorType, equip_type, item_name, color);
        if (itemName == "none") {
            zone.destroy(item_ID);
            Utilities::Log("Create_And_Equip_Armor() " + itemName + " no item in db, no item has been created");
            return Item_Component::emptyEquipSlot[state];
        } else {
            Create_Item1(zone, item_ID, position, itemName, itemStats);
            Utilities::Log("Create_And_Equip_Armor() " + itemName + " item has successfully been created");
            return item_ID;
        }
    }

    entt::entity Create_And_Equip_Cosmetic(entt::registry &zone, int &state, Component::Position &position, Item_Component::Item_Type itemType, Item_Component::Unit_Equip_Type &equip_type, Item_Component::Item item_name, SDL_Color color) {
        Rarity rarity = Generate_Item_Rarity();
        Item_Stats itemStats = {};
        Armor_Type armorType = Items::Generate_Armor_Type();

        auto item_ID = zone.create();
        std::string itemName = Create_Specific_Armor(zone, item_ID, rarity, itemType, armorType, equip_type, item_name, color);
        if (itemName == "none") {
            zone.destroy(item_ID);
            Utilities::Log("Create_And_Equip_Armor() " + itemName + " no item in db, no item has been created");
            return Item_Component::emptyEquipSlot[state];
        } else {
            Create_Item1(zone, item_ID, position, itemName, itemStats);
            Utilities::Log("Create_And_Equip_Armor() " + itemName + " item has successfully been created");
            return item_ID;
        }
    }

    //mouse click
    //walk over to item
    //when arrived at item pick it up into mouse array
    void Hightlight_Item_Under_Cursor(SDL_FRect &itemRect) {
        if (Mouse::bRect_inside_Cursor(itemRect)) {//if cursor in inside item text box
            //then change the background color of that box to another shade
            Color::Set_Render_Draw_Color(Graphics::renderer, Color::darkBlue, 155);
        } else {
            //set beackground of textbox to black
            Color::Set_Render_Draw_Color(Graphics::renderer, Color::black);
        }
    }

    void Item_Collision(entt::registry &zone) {
        ///What we actually need to do is create a quad tree with each cell the size of one item and only one item per cell
        /// when an item it dropped it falls it queries the tree into the nearest vacant cell
        auto view = zone.view<Item_Component::Ground_Item, Component::Position, Item_Component::Name, Component::Renderable>();
        for (auto item1: view) {
            auto &itemPosition1 = view.get<Component::Position>(item1);
            auto &name1 = view.get<Name>(item1).name;
            auto &groundItem1 = view.get<Ground_Item>(item1);
            SDL_Rect textBox1 = {};
            textBox1.w = name1.length() * 5;
            textBox1.h = 10;
            textBox1.x = itemPosition1.x - (textBox1.w / 2.0f);
            textBox1.y = itemPosition1.y - 10;
            for (auto item2: view) {
                if (item1 != item2) {
                    auto &itemPosition2 = view.get<Component::Position>(item2);
                    auto &name2 = view.get<Name>(item2).name;
                    auto &groundItem2 = view.get<Ground_Item>(item2);
                    SDL_Rect textBox2 = {};
                    textBox2.w = name2.length() * 5;
                    textBox2.h = 10;
                    textBox2.x = itemPosition2.x - (textBox2.w / 2.0f);
                    textBox2.y = itemPosition2.y - 10;
                    SDL_Point rectPosition = Utilities::Check_Collision_Rects(textBox1, textBox2);
                    groundItem1.ground_name.x += rectPosition.x;
                    groundItem1.ground_name.y += rectPosition.y;
                    groundItem2.ground_name.x -= rectPosition.x;
                    groundItem2.ground_name.y -= rectPosition.y;
                }
            }
        }
    }

    void Update_Mouse_Slot_Position(entt::registry &zone, const int &state, const entt::entity &item, const bool &isItemCurrentlyHeld, const float &mouseX, const float &mouseY) {
        //set item in mouse array position to mouse x, y every frame
        if (isItemCurrentlyHeld) {
            if (zone.any_of<Component::Position>(item)) {
                if (Mouse_Struct::mouseData.type == Component::Icon_Type::building) {
                    auto &rect = zone.get<Component::Interaction_Rect>(item).rect;
                    rect.x = mouseX - (rect.w / 2.0f);
                    rect.y = mouseY - (rect.h / 2.0f);
                    auto &placeable = zone.get<Building_Component::Placement>(item);
                    for (const auto &polygon: placeable.polygons) {
                        Building_Component::Polygon treePolygon;
                        for (auto vec: polygon) {
                            treePolygon.push_back({mouseX + vec.x, mouseY + vec.y});
                        }

                        auto name = zone.get<Rendering_Components::Interior_Sheet_Info>(item).collisionBocArrayIndex;
                        Collision::Save_Line_Segment(zone, item, name);

                        std::vector<Building_Component::Polygon> place;
                        place.emplace_back(treePolygon);
                        zone.emplace_or_replace<Building_Component::Set_Placement>(item, place);

                        auto view = zone.view<Component::Camera>();
                        for (auto entity: view) {
                            auto &camera = view.get<Component::Camera>(entity);
                            SDL_FPoint points[4];
                            for (int j = 0; j < 4; ++j) {
                                points[j].x = treePolygon[j].x - camera.screen.x;
                                points[j].y = treePolygon[j].y - camera.screen.y;
                            }
                            SDL_RenderDrawLinesF(Graphics::renderer, points, 4);
                            SDL_RenderDrawLineF(Graphics::renderer, points[0].x, points[0].y, points[3].x, points[3].y);
                        }

                        placeable.obstructed = Quad_Tree::Entity_vs_Polygon_Collision(zone, state, treePolygon, rect);
                    }

                    auto &sprite = zone.get<Rendering_Components::Sprite_Sheet_Info>(item);
                    sprite.blendType = Rendering_Components::ghost;
                    if (placeable.obstructed) {
                        sprite.color = Color::Set_Color_With_Alpha(Color::red, 50);
                    } else {
                        sprite.color = Color::Set_Color_With_Alpha(Color::gray, 50);
                    }
                }

                Component::Position &position = zone.get<Component::Position>(item);
                position.x = mouseX;
                position.y = mouseY;
            } else {
                //        Utilities::Log("itemID " + std::to_string((int)item));
            }
        }
        //		Item_Collision(zone);
    }

    void Show_Ground_Items(entt::registry &zone, Component::Camera &camera) {
        if (showGroundItems) {
            //****//search quad tree instead
            auto view = zone.view<Ground_Item, Component::Interaction_Rect, Rarity, Name, Component::Renderable>();
            for (auto item: view) {
                ///need position for render location
                /// need an offset based on rect size for the text box and item position
                ///
                ///need name for string
                /// need rarity for colour
                auto &interactionRect = view.get<Component::Interaction_Rect>(item);
                Component::Position itemPosition = {interactionRect.rect.x + (interactionRect.rect.w / 2.0f), interactionRect.rect.y};

                auto &rarity = view.get<Rarity>(item);
                auto &name = view.get<Name>(item).name;
                Graphics::Text_Box_Data itemTextBox = Graphics::Create_Text_Background(camera, rarityColor[rarity], name, itemPosition);
                auto &highlightBox = view.get<Ground_Item>(item).ground_name;
                highlightBox = Utilities::SDL_Rect_To_SDL_FRect(itemTextBox.textBoxBackground);
                Hightlight_Item_Under_Cursor(highlightBox);
                SDL_RenderFillRect(Graphics::renderer, &itemTextBox.textBoxBackground);
                SDL_RenderCopyF(Graphics::renderer, itemTextBox.textdata.pTexture, &itemTextBox.textdata.k, &itemTextBox.highlightBox);
                SDL_DestroyTexture(itemTextBox.textdata.pTexture);
            }
        }
        //run rect collsion to break up the item names
    }

    void Name_On_Mouseover(entt::registry &zone, Component::Camera &camera) {
        auto view = zone.view<Ground_Item, Component::Interaction_Rect, Rarity, Name, Component::Renderable>();
        for (auto item: view) {
            auto &box = view.get<Ground_Item>(item);
            if (Mouse::FRect_inside_Cursor(box.box)) {
                //****//search quad tree instead
                ///need position for render location
                /// need an offset based on rect size for the text box and item position
                ///
                ///need name for string
                /// need rarity for colour
                auto &interactionRect = view.get<Component::Interaction_Rect>(item);
                Component::Position itemPosition = {interactionRect.rect.x + (interactionRect.rect.w / 2.0f), interactionRect.rect.y};

                auto &rarity = view.get<Rarity>(item);
                auto &name = view.get<Name>(item).name;
                Graphics::Text_Box_Data itemTextBox = Graphics::Create_Text_Background(camera, rarityColor[rarity], name, itemPosition);

                SDL_RenderFillRect(Graphics::renderer, &itemTextBox.textBoxBackground);
                SDL_RenderCopyF(Graphics::renderer, itemTextBox.textdata.pTexture, &itemTextBox.textdata.k, &itemTextBox.highlightBox);
                SDL_DestroyTexture(itemTextBox.textdata.pTexture);
            }
        }
    }


    void Unit_Name_On_Mouseover(entt::registry &zone, Component::Camera &camera) {
        auto view = zone.view<Component::Unit, Component::Name, Component::Interaction_Rect, Component::Renderable>();
        for (auto item: view) {
            auto &interactionRect = view.get<Component::Interaction_Rect>(item);
            if (Mouse::FRect_inside_Cursor(interactionRect.rect)) {
                //****//search quad tree instead
                ///need position for render location
                /// need an offset based on rect size for the text box and item position
                ///
                ///need name for string
                /// need rarity for colour
                Component::Position itemPosition = {interactionRect.rect.x + (interactionRect.rect.w / 2.0f), interactionRect.rect.y};

                auto &name = view.get<Component::Name>(item);
                std::string full_name;
                if (name.nickname == "") {
                    full_name = name.first + " " + name.last;
                } else {
                    full_name = name.nickname;
                }
                Graphics::Text_Box_Data itemTextBox = Graphics::Create_Text_Background(camera, rarityColor[Item_Component::Rarity::common], full_name, itemPosition);
                //                auto& highlightBox = view.get<Ground_Item>(item).ground_name;
                //                highlightBox = Utilities::SDL_Rect_To_SDL_FRect(itemTextBox.textBoxBackground);
                SDL_RenderFillRect(Graphics::renderer, &itemTextBox.textBoxBackground);
                SDL_RenderCopyF(Graphics::renderer, itemTextBox.textdata.pTexture, &itemTextBox.textdata.k, &itemTextBox.highlightBox);
                SDL_DestroyTexture(itemTextBox.textdata.pTexture);
            }
        }
    }

    void Init_Item_Data() {

        SQLite_Item_Data::Load_Item_Names();

        rarityColor = {
                {Rarity::common, Color::white},
                {Rarity::magic,  Color::blueMagical},
                {Rarity::rare,   Color::yellowMagical},
                {Rarity::unique, Color::redMagical},
        };

        rarityBorder = {
                {Rarity::common, Texture::itemBorderCommon},
                {Rarity::magic,  Texture::itemBorderMagic},
                {Rarity::rare,   Texture::itemBorderRare},
                {Rarity::unique, Texture::itemBorderEite},
        };

        itemTypes = {
                {Item_Type::helm,    0},
                {Item_Type::chest,   1},
                {Item_Type::gloves,  2},
                {Item_Type::legs,    3},
                {Item_Type::boots,   4},
                //{Item_Type::neck, 1},
                //{Item_Type::shoulders, 2},
                //{Item_Type::belt, 6},
                {Item_Type::hair,    5},
//                {Item_Type::kilt,    6},
                {Item_Type::offhand, 7}};

        armorTypes = {
                {Armor_Type::cloth,   0},
                {Armor_Type::padded,  1},
                {Armor_Type::leather, 2},
                {Armor_Type::mail,    3},
                {Armor_Type::plate,   4}};

        weaponTypes = {
                {Weapon_Type::sword, 0},
                {Weapon_Type::axe,   1},
                {Weapon_Type::mace,  2},
                {Weapon_Type::spear, 3}};

        offhandTypes = {
                {Offhand_Type::dagger, 0},
                {Offhand_Type::kama,   1},
                {Offhand_Type::shield, 2},
        };

        weaponMaterials = {
                {Weapon_Material::copper, 0},
                {Weapon_Material::bronze, 1},
                {Weapon_Material::iron,   2},
        };

        ArmorTypeName = {
                {Item_Component::Armor_Type::cloth,   "cloth"},
                {Item_Component::Armor_Type::padded,  "padded"},
                {Item_Component::Armor_Type::leather, "leather"},
                {Item_Component::Armor_Type::mail,    "mail"},
                {Item_Component::Armor_Type::plate,   "plate"}};

        ItemTypeName = {
                {Item_Component::Item_Type::helm,   "helm"},
                {Item_Component::Item_Type::chest,  "chest"},
                {Item_Component::Item_Type::gloves, "gloves"},
                {Item_Component::Item_Type::legs,   "legs"},
                {Item_Component::Item_Type::boots,  "boots"},
                {Item_Component::Item_Type::hair,   "hair"},
//                {Item_Component::Item_Type::kilt,   "kilt"},
        };

        weaponMaterialName = {
                {Item_Component::Weapon_Material::copper, "copper"},
                {Item_Component::Weapon_Material::bronze, "bronze"},
                {Item_Component::Weapon_Material::iron,   "iron"},
        };

        weaponTypeName = {
                {Item_Component::Weapon_Type::sword, "sword"},
                {Item_Component::Weapon_Type::axe,   "axe"},
                {Item_Component::Weapon_Type::mace,  "mace"},
                {Item_Component::Weapon_Type::spear, "polearm"}};

        offhandTypeName = {
                {Item_Component::Offhand_Type::dagger, "dagger"},
                {Item_Component::Offhand_Type::kama,   "kama"},
                {Item_Component::Offhand_Type::shield, "shield"},
        };

        baseStatData = {
                {Item_Component::Stat::health,      0},
                {Item_Component::Stat::damage,      1},
                {Item_Component::Stat::spellDamage, 0},
                {Item_Component::Stat::armor,       0},
                {Item_Component::Stat::piety,       0},
                {Item_Component::Stat::attackSpeed, 500},
        };

        statData = baseStatData;

        statName = {
                {Item_Component::Stat::health,      "Health"},
                {Item_Component::Stat::damage,      "Damage"},
                {Item_Component::Stat::spellDamage, "Spell Damage"},
                {Item_Component::Stat::armor,       "Armour"},
                {Item_Component::Stat::piety,       "Piety"},
                {Item_Component::Stat::attackSpeed, "Attack Speed"},
        };
    }
}// namespace Items
