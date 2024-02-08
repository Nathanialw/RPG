#pragma once
#include "character_options.h"
#include "create_entities.h"
#include "graphics.h"
#include "items.h"
#include "load_zone.h"
#include "ui.h"
#include <SDL2/SDL.h>
#include <sstream>

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
  void Update_Equip_slots(entt::registry &zone, int &state) {// run funtion on item equip or unequip
    auto view = zone.view<Rendering_Components::Sprite_Sheet_Info, Item_Component::Item_Equip, Item_Component::Equipment, Rendering_Components::Equipment_Sprites>();
    for (auto entity: view) {
      auto &equipment = view.get<Item_Component::Equipment>(entity);
      auto &equipmentSprites =
          view.get<Rendering_Components::Equipment_Sprites>(entity);

      // iterate through each equip slot
      for (auto &item: equipment.equippedItems) {
        if (item.second != Item_Component::emptyEquipSlot[state]) {

          // get the item at the item type index
          auto &weaponSheet = zone.get<Rendering_Components::Sprite_Sheet_Info>(item.second);
          if (weaponSheet.sheetData) {
            equipmentSprites.sheet[(int) item.first].ItemSheetData = weaponSheet.sheetData;
            equipmentSprites.sheet[(int) item.first].name = weaponSheet.sheet_name;
            equipmentSprites.sheet[(int) item.first].itemID = item.second;
            equipmentSprites.sheet[(int) item.first].color = weaponSheet.color;
          }
        }

        else if (item.second == Item_Component::emptyEquipSlot[state]) {
          equipmentSprites.sheet[(int) item.first].ItemSheetData = NULL;
          if ((int) item.first < (int) Item_Component::Item_Type::SIZE) {
            Utilities::Log((int) item.first);
            Utilities::Log(equipmentSprites.sheet[(int) item.first].name);
            Utilities::Log((int) item.second);
            equipmentSprites.sheet[(int) item.first].name = "empty";
            equipmentSprites.sheet[(int) item.first].itemID = emptyEquipSlot[state];
          } else {
            Utilities::Log("-------");
            Utilities::Log((int) item.first);
            Utilities::Log("-------");
          }
        }
      }
    }
  }

  void Update_Unit_Stats(entt::registry &zone, int &state) {
    auto view = zone.view<Item_Component::Item_Equip, Component::Melee_Damage, Component::Attack_Speed, Component::Health, Item_Component::Equipment>();
    for (auto entity: view) {
      auto &damage = view.get<Component::Melee_Damage>(entity);
      auto &attackSpeed = view.get<Component::Attack_Speed>(entity);
      auto &health = view.get<Component::Health>(entity);
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
        if (item.second != Item_Component::emptyEquipSlot[state]) {
          auto &stats = zone.get<Item_Stats>(item.second).stats;
          for (auto &stat: stats) {
            Item_Component::statData[stat.first] += stat.second;
            Utilities::Log(std::to_string(Item_Component::statData[stat.first]) + ": " + std::to_string(stat.second));
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
            health.maxHealth = stat.second;
            break;
          case Item_Component::Stat::piety:
            break;
        }
      }
      zone.remove<Item_Component::Item_Equip>(entity);
    }
  }

  struct Stat_Textures {
    SDL_Texture *stat;
    SDL_Texture *value;
  };
  std::unordered_map<std::string, Stat_Textures> statsValues;

  void Render_Stat(SDL_FRect &statBox, float &charHeight, float &charWidth, float &currentRow, SDL_Color &black, std::pair<const Stat, int> stat) {
    Graphics::Surface_Data statNameData = Graphics::Load_Text_Texture(Item_Component::statName[stat.first], black);

    SDL_FRect statNameRect = statBox;
    statNameRect.y = statBox.y + currentRow;
    statNameRect.h = charHeight;
    statNameRect.w = Item_Component::statName[stat.first].size() * charWidth;

    Graphics::Render_FRect(statNameData.pTexture, black, &statNameData.k, &statNameRect);
    SDL_DestroyTexture(statNameData.pTexture);

    std::string statValue = std::to_string(stat.second);
    Graphics::Surface_Data statValueData = Graphics::Load_Text_Texture(statValue, black);

    SDL_FRect statValueRect = statBox;
    statValueRect.y = statBox.y + currentRow;
    statValueRect.h = charHeight;
    statValueRect.w = statValue.size() * charWidth;
    statValueRect.x = statBox.x + statBox.w - statValueRect.w;

    Graphics::Render_FRect(statValueData.pTexture, black, &statValueData.k, &statValueRect);
    SDL_DestroyTexture(statValueData.pTexture);

    statValueRect.x = statValueRect.x + (statBox.w - (statValue.length() * charWidth));
    statBox.y += charHeight;
  }

  void Render_Stat_FC(Component::Camera &camera, SDL_FRect &statBox, float &charHeight, float &charWidth, SDL_Color color, std::pair<const Stat, int> stat) {
    FC_Scale scale = {1.0f / camera.scale.x, 1.0f / camera.scale.y};

    SDL_FRect statNameRect = statBox;
    FC_DrawScale(Graphics::fcFont, Graphics::renderer, statNameRect.x, statNameRect.y, scale, color, Item_Component::statName[stat.first].c_str());

    std::string statValue = std::to_string(stat.second);
    SDL_FRect statValueRect = statBox;
    statValueRect.x = statValueRect.x + statBox.w;

    FC_DrawScaleRight(Graphics::fcFont, Graphics::renderer, statValueRect.x, statValueRect.y, scale, statValue.c_str());

    statBox.y += charHeight;
  }

  void Render_Character_Stats(Component::Camera &camera) {
    if (UI::bToggleCharacterUI) {

      SDL_FRect statsBox = {UI::defaultScreenPosition.x + statsSheetOffsetRect.x, UI::defaultScreenPosition.y + statsSheetOffsetRect.y, statsSheetOffsetRect.w, statsSheetOffsetRect.h};
      SDL_FRect statBox = Camera_Control::Convert_FRect_To_Scale(statsBox, camera);
      SDL_Color black = {0, 0, 0, 255};

      float currentRow = 0.0f;
      float charHeight = (20.0f / camera.scale.y);
      float charWidth = (10.0f / camera.scale.x);

      for (auto &stat: Item_Component::statData) {
        Debug::settings[Debug::Settings::fontRenderFC] ? Render_Stat(statBox, charHeight, charWidth, currentRow, black, stat) : Render_Stat_FC(camera, statBox, charHeight, charWidth, black, stat);
      }
    }
  }

  void Init_Player_Stats(entt::registry &zone, int &state) {// run funtion on item equip or unequip
    auto view = zone.view<Component::Input, Component::Melee_Damage, Component::Health, Component::Attack_Speed, Item_Component::Equipment>();
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
        if (item.second != Item_Component::emptyEquipSlot[state]) {
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


  void Equip_Units(entt::registry &zone, int &state, Character_Options::Customization &options) {
    std::vector<std::string> gear = Get_Sex(options.sex);
    Item_Component::Item hair = Get_Hair_Name(options);
    Item_Component::Item beard = Get_Beard_Name(options);
    Item_Component::Item horns = Get_Horn_Name(options);


    auto view = zone.view<Item_Component::Equipment, Component::Position, Rendering_Components::Unit_Frame_Portrait, Rendering_Components::Body_Frame, Action_Component::Action>();
    for (auto unit: view) {
      auto &equipment = view.get<Item_Component::Equipment>(unit);
      auto &position = view.get<Component::Position>(unit);
      auto &unitPortraitFrame = view.get<Rendering_Components::Unit_Frame_Portrait>(unit);
      auto &bodyFrame = view.get<Rendering_Components::Body_Frame>(unit);
      auto &action = view.get<Action_Component::Action>(unit);

      entt::entity item = Items::Create_And_Equip_Weapon(zone, state, position, equipment.type, SQLite_Item_Data::Load_Specific_Item(gear[0].c_str()), Character_Options::Color[0]);
      equipment.equippedItems[Item_Type::mainhand] = item;
      action.weaponType = zone.get<Item_Component::Weapon_Type>(item);
      Load::Get_Bust_Textures(zone, state, item, Item_Type::mainhand, bodyFrame, unitPortraitFrame);

      item = Items::Create_And_Equip_Armor(zone, state, position, Item_Type::chest, equipment.type, SQLite_Item_Data::Load_Specific_Item(gear[1].c_str()), Character_Options::Color[0]);
      equipment.equippedItems[Item_Type::chest] = item;
      Load::Get_Bust_Textures(zone, state, item, Item_Type::chest, bodyFrame, unitPortraitFrame);

      item = Items::Create_And_Equip_Armor(zone, state, position, Item_Type::legs, equipment.type, SQLite_Item_Data::Load_Specific_Item(gear[2].c_str()), Character_Options::Color[0]);
      equipment.equippedItems[Item_Type::legs] = item;
      Load::Get_Bust_Textures(zone, state, item, Item_Type::legs, bodyFrame, unitPortraitFrame);

      item = Items::Create_And_Equip_Cosmetic(zone, state, position, Item_Type::hair, equipment.type, hair, Character_Options::Color[options.hairColor]);
      equipment.equippedItems[Item_Type::hair] = item;
      Load::Get_Bust_Textures(zone, state, item, Item_Type::hair, bodyFrame, unitPortraitFrame);

      item = Items::Create_And_Equip_Cosmetic(zone, state, position, Item_Type::facialHair, equipment.type, beard, Character_Options::Color[options.hairColor]);
      equipment.equippedItems[Item_Type::facialHair] = item;
      Load::Get_Bust_Textures(zone, state, item, Item_Type::facialHair, bodyFrame, unitPortraitFrame);

      if (horns.hasTexture) {
        item = Items::Create_And_Equip_Cosmetic(zone, state, position, Item_Type::horns, equipment.type, horns, Character_Options::Color[0]);
        equipment.equippedItems[Item_Type::horns] = item;
        Load::Get_Bust_Textures(zone, state, item, Item_Type::horns, bodyFrame, unitPortraitFrame);
      }

      zone.emplace_or_replace<Item_Component::Item_Equip>(unit);
    }
  }

  void Update_Items(entt::registry &zone, int &state) {
    Update_Equip_slots(zone, state);
    Update_Unit_Stats(zone, state);
  }

  void Init_Items() {
    Items::Init_Item_Data();
  }

  void Init_Player(entt::registry &zone, int &state, Character_Options::Customization &options) {
    db::Unit_Data data = Entity_Loader::Get_Character_Create(Character_Options::Get_Character(options));
    Component::Unit_Index unitIndex = {"demons", 0};

    Social_Component::Summon summon;
    Create_Entities::Create_Entity(zone, state, 1660, 8924, data, true, summon, unitIndex);
    Equip_Units(zone, state, options);
    Init_Player_Stats(zone, state);
  }

  void Recreate_Player(entt::registry &zone, int &state, Character_Options::Customization &options) {
    db::Unit_Data data = Entity_Loader::Get_Character_Create(Character_Options::Get_Character(options));
    Social_Component::Summon summon;
    Component::Unit_Index unitIndex = {"demons", 0};

    Create_Entities::Create_Entity(zone, state, 1660, 8924, data, true, summon, unitIndex);
    Init_Player_Stats(zone, state);
  }

}// namespace Character_Stats
