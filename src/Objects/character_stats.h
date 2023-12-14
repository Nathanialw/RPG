#pragma once
#include "graphics.h"
#include "items.h"
#include "ui.h"
#include <SDL2/SDL.h>
#include <sstream>
#include "character_options.h"

namespace Character_Stats {

  SDL_FRect statsSheetOffsetRect = {160.0f, 64.0f, 240.0f, 384.0f};

  SDL_FRect screenCharacterStats = {
    UI::defaultScreenPosition.x + statsSheetOffsetRect.x,
    UI::defaultScreenPosition.y + statsSheetOffsetRect.y,
    statsSheetOffsetRect.w, statsSheetOffsetRect.h};

    Graphics::Surface_Data Create_Text_Box(std::string &string, SDL_Color &rarity, SDL_Rect &textBox) {
    Graphics::Surface_Data text = Graphics::Load_Text_Texture(string, rarity);
    text.k = textBox;
    return text;
  }

  // update all stats
  // cycle through each equip slot and adds the stats to the base stats
  int Update_Stat(int characterStat, int itemStat) {
    return itemStat + characterStat;
  }

  // run when equipping or unequipping an item
  void Update_Equip_slots(
      entt::registry &zone) {// run funtion on item equip or unequip
    auto view = zone.view<Rendering_Components::Sprite_Sheet_Info,
                          Item_Component::Item_Equip, Item_Component::Equipment,
                          Rendering_Components::Equipment_Sprites>();
    for (auto entity: view) {
      auto &equipment = view.get<Item_Component::Equipment>(entity);
      auto &equipmentSprites =
          view.get<Rendering_Components::Equipment_Sprites>(entity);

      // iterate through each equip slot
      for (auto &item: equipment.equippedItems) {
        if (item.second != Item_Component::emptyEquipSlot) {

          // get the item at the item type index
          auto &weaponSheet =
              zone.get<Rendering_Components::Sprite_Sheet_Info>(item.second);
          if (weaponSheet.sheetData) {
            equipmentSprites.sheet[(int) item.first].ItemSheetData =
                weaponSheet.sheetData;
            equipmentSprites.sheet[(int) item.first].name = weaponSheet.sheet_name;
            equipmentSprites.sheet[(int) item.first].itemID = item.second;
            equipmentSprites.sheet[(int) item.first].color = weaponSheet.color;
          }
        }

        else if (item.second == Item_Component::emptyEquipSlot) {
          equipmentSprites.sheet[(int) item.first].ItemSheetData = NULL;
          equipmentSprites.sheet[(int) item.first].name = "empty";
          equipmentSprites.sheet[(int) item.first].itemID = emptyEquipSlot;
        }
      }
    }
  }

  void Update_Unit_Offense(entt::registry &zone) {
    auto view = zone.view<Item_Component::Item_Equip, Component::Melee_Damage,
                          Component::Attack_Speed, Item_Component::Equipment>();
    for (auto entity: view) {
      auto &damage = view.get<Component::Melee_Damage>(entity);
      auto &attackSpeed = view.get<Component::Attack_Speed>(entity);
      auto &equipment = view.get<Item_Component::Equipment>(entity);

      // set character stats to base values
      for (auto &stat: Item_Component::statData) {
        if (stat.first == Item_Component::Stat::damage) {
          stat.second = Item_Component::baseStatData[stat.first];
        }
        if (stat.first == Item_Component::Stat::attackSpeed) {
          stat.second = Item_Component::baseStatData[stat.first];
        }
        if (stat.first == Item_Component::Stat::spellDamage) {
          stat.second = Item_Component::baseStatData[stat.first];
        }
      }

      // check if slot is occupied, add stats if it is
      for (auto &item: equipment.equippedItems) {
        if (item.second != Item_Component::emptyEquipSlot) {
          auto &stats = zone.get<Item_Stats>(item.second).stats;
          for (auto &stat: stats) {
            Item_Component::statData[stat.first] += stat.second;
          }
        }
      }

      // update components from updated character stat data
      for (auto &stat: Item_Component::statData) {
        switch (stat.first) {
          case Item_Component::Stat::damage:
            damage.minDamage = stat.second;
            damage.maxDamage = stat.second;
            break;
          case Item_Component::Stat::attackSpeed:
            attackSpeed.period = stat.second;
            break;
          case Item_Component::Stat::armor:
            break;
          case Item_Component::Stat::spellDamage:
            break;
          case Item_Component::Stat::health:
            break;
          case Item_Component::Stat::piety:
            break;
        }
      }
    }
  }

  void Update_Unit_Defense(entt::registry &zone) {
    auto view = zone.view<Item_Component::Item_Equip, Component::Health,
                          Item_Component::Equipment>();
    for (auto entity: view) {
      auto &health = view.get<Component::Health>(entity);
      auto &equipment = view.get<Item_Component::Equipment>(entity);

      // set character stats to base values
      for (auto &stat: Item_Component::statData) {
        if (stat.first == Item_Component::Stat::health) {
          stat.second = Item_Component::baseStatData[stat.first];
        }
        if (stat.first == Item_Component::Stat::armor) {
          stat.second = Item_Component::baseStatData[stat.first];
        }
        if (stat.first == Item_Component::Stat::piety) {
          stat.second = Item_Component::baseStatData[stat.first];
        }
      }

      // check if slot is occupied, add stats if it is
      for (auto &item: equipment.equippedItems) {
        if (item.second != Item_Component::emptyEquipSlot) {
          auto &stats = zone.get<Item_Stats>(item.second).stats;
          for (auto &stat: stats) {
            Item_Component::statData[stat.first] += stat.second;
          }
        }
      }

      // update components from updated character stat data
      for (auto &stat: Item_Component::statData) {
        switch (stat.first) {
          case Item_Component::Stat::health:
            health.maxHealth = stat.second;
            break;
          case Item_Component::Stat::armor:
            break;
          case Item_Component::Stat::spellDamage:
            break;
          case Item_Component::Stat::damage:
            break;
          case Item_Component::Stat::attackSpeed:
            break;
          case Item_Component::Stat::piety:
            break;
        }
      }
      zone.remove<Item_Component::Item_Equip>(entity);
    }
  }

  void Render_Character_Stats(Component::Camera &camera) {
    if (UI::bToggleCharacterUI) {

      SDL_FRect statsBox = {UI::defaultScreenPosition.x + statsSheetOffsetRect.x,
                            UI::defaultScreenPosition.y + statsSheetOffsetRect.y,
                            statsSheetOffsetRect.w, statsSheetOffsetRect.h};

      SDL_FRect statBox = Camera_Control::Convert_FRect_To_Scale(statsBox, camera);
      SDL_Color black = {0, 0, 0, 255};

      float currentRow = 0.0f;
      float charHeight = (20.0f / camera.scale.y);
      float charWidth = (10.0f / camera.scale.x);

      for (auto &stat: Item_Component::statData) {
        // Graphics::Surface_Data statNameData = Graphics::Load_Text_Texture(Items::statName[stat.first], black);

        SDL_FRect statNameRect = statBox;
        statNameRect.y = statBox.y + currentRow;
        // statNameRect.h = charHeight;
        // statNameRect.w = Items::statName[stat.first].size() * charWidth;

        FC_Draw(Graphics::fcFont, Graphics::renderer, statNameRect.x,
                statNameRect.y, Item_Component::statName[stat.first].c_str());
        // Graphics::Render_FRect(statNameData.pTexture, &statNameData.k, &statNameRect); SDL_DestroyTexture(statNameData.pTexture);
        ////SDL_RenderDrawRect(Graphics::renderer, &rowRect);
        //
        std::string statValue = std::to_string(stat.second);
        // Graphics::Surface_Data statValueData = Graphics::Load_Text_Texture(statValue, black);
        //
        SDL_FRect statValueRect = (statBox);
        statValueRect.y = statBox.y + currentRow;
        statValueRect.x = statBox.x + statBox.w - statValueRect.w;
        // statValueRect.h = charHeight;
        // statValueRect.w = statValue.size() * charWidth;
        //
        //
        // Graphics::Render_FRect(statValueData.pTexture, &statValueData.k, &statValueRect); SDL_DestroyTexture(statValueData.pTexture); currentRow += charHeight;
        FC_Draw(Graphics::fcFont, Graphics::renderer, statValueRect.x, statValueRect.y, statValue.c_str());
      }
    }
  }

  void Init_Player_Stats(
      entt::registry &zone) {// run funtion on item equip or unequip
    auto view =
        zone.view<Component::Input, Component::Melee_Damage, Component::Health,
                  Component::Attack_Speed, Item_Component::Equipment>();
    for (auto entity: view) {
      auto &damage = view.get<Component::Melee_Damage>(entity);
      auto &health = view.get<Component::Health>(entity);
      auto &attackSpeed = view.get<Component::Attack_Speed>(entity);
      auto &equipment = view.get<Item_Component::Equipment>(entity);

      // set charcter stats to base values
      for (auto &stat: Item_Component::statData) {
        stat.second = Item_Component::baseStatData[stat.first];
      }

      // add equipment stats to charcter stats
      for (auto &item: equipment.equippedItems) {
        if (item.second != Item_Component::emptyEquipSlot) {
          auto &stats = zone.get<Item_Stats>(item.second).stats;
          for (auto &stat: stats) {
            Item_Component::statData[stat.first] += stat.second;
          }
        }
      }

      // update components from updated character stat data
      for (auto &stat: Item_Component::statData) {
        switch (stat.first) {
          case Item_Component::Stat::damage:
            damage.minDamage = stat.second;
            damage.maxDamage = stat.second;
            break;
          case Item_Component::Stat::health:
            health.maxHealth = stat.second;
            break;
          case Item_Component::Stat::attackSpeed:
            attackSpeed.period = stat.second;
            break;
          case Item_Component::Stat::spellDamage:
            break;
          case Item_Component::Stat::armor:
            break;
          case Item_Component::Stat::piety:
            break;
        }
      }
    }
  }

  void Equip_Units(entt::registry &zone, Character_Options::Customization &options) {
    std::vector<std::string> gear = Get_Sex(options.sex);
    std::vector<std::string> hair = Get_Hair(options.sex);
    auto view = zone.view<Item_Component::Equipment, Component::Position>();
    for (auto unit: view) {
      auto &equipment = view.get<Item_Component::Equipment>(unit);
      auto &position = view.get<Component::Position>(unit);

      equipment.equippedItems[Item_Type::mainhand] = Items::Create_And_Equip_Weapon(position, equipment.type, gear[0]);
      equipment.equippedItems[Item_Type::chest] = Items::Create_And_Equip_Armor(position, Item_Type::chest, equipment.type, gear[1], Character_Options::Color[0]);
      equipment.equippedItems[Item_Type::legs] = Items::Create_And_Equip_Armor(position, Item_Type::legs, equipment.type, gear[2], Character_Options::Color[0]);
      equipment.equippedItems[Item_Type::hair] = Items::Create_And_Equip_Armor(position, Item_Type::hair, equipment.type, hair[options.hairStyle], Character_Options::Color[options.hairColor]);

      zone.emplace<Item_Component::Item_Equip>(unit);
    }
  }

  void Update_Items(entt::registry &zone) {
    Update_Equip_slots(zone);
    Update_Unit_Offense(zone);
    // defensive has to come after offensive as it removes the Item_Equip component
    Update_Unit_Defense(zone);
  }

  void Init_Items(entt::registry &zone) {
    Items::Init_Item_Data();
  }

  void Init_UI(entt::registry &zone) {
    UI::Bag_UI::Create_Bag_UI(zone);
    UI::Equipment_UI::Create_Equipment_UI(zone);
  }

  void Init_Player(entt::registry &zone, Character_Options::Customization &options) {
    Character_Options::Race race = Character_Options::Get_Gender(options);
    Create_Entities::Create_Entity(zone, 73188, 36964, race.templateName, "unit", false, race.path, true);
    Equip_Units(zone, options);
    Init_Player_Stats(zone);
  }
}
