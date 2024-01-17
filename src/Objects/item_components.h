#pragma once
#include "entt/entt.hpp"
#include <SDL2/SDL.h>
#include <map>

namespace Item_Component {

  //    item 0 renders first
  enum class Item_Type {
    clothes,
    amulet,
    legs,
    belt,
    wrist,
    boots,
    kilt,
    chest,
    hair,
    gloves,
    ring,
    facialHair,
    face,
    hood,
    helm,
    crown,
    shins,
    back,
    shoulders,
    jewelry,
    dirt,
    ranged,
    mainhand,
    offhand,
    horns,
    size
  };

  Item_Type Get_Item_Type(std::string &db_type) {
    if (db_type == "back") {
      return Item_Type::back;
    } else if (db_type == "mainhand") {
      return Item_Type::mainhand;
    } else if (db_type == "amulet") {
      return Item_Type::amulet;
    } else if (db_type == "face") {
      return Item_Type::face;
    } else if (db_type == "facialHair") {
      return Item_Type::facialHair;
    } else if (db_type == "belt") {
      return Item_Type::belt;
    } else if (db_type == "boots") {
      return Item_Type::boots;
    } else if (db_type == "ranged") {
      return Item_Type::ranged;
    } else if (db_type == "chest") {
      return Item_Type::chest;
    } else if (db_type == "clothes") {
      return Item_Type::clothes;
    } else if (db_type == "crown") {
      return Item_Type::crown;
    } else if (db_type == "dirt") {
      return Item_Type::dirt;
    } else if (db_type == "gloves") {
      return Item_Type::gloves;
    } else if (db_type == "hair") {
      return Item_Type::hair;
    } else if (db_type == "helm") {
      return Item_Type::helm;
    } else if (db_type == "hood") {
      return Item_Type::hood;
    } else if (db_type == "kilt") {
      return Item_Type::kilt;
    } else if (db_type == "legs") {
      return Item_Type::legs;
    } else if (db_type == "ring") {
      return Item_Type::ring;
    } else if (db_type == "offhand") {
      return Item_Type::offhand;
    } else if (db_type == "shield") {
      return Item_Type::offhand;
    } else if (db_type == "shins") {
      return Item_Type::shins;
    } else if (db_type == "shoulders") {
      return Item_Type::shoulders;
    } else if (db_type == "wrist") {
      return Item_Type::wrist;
    } else if (db_type == "jewelry") {
      return Item_Type::jewelry;
    } else if (db_type == "horns") {
      return Item_Type::horns;
    } else {
      Utilities::Log("Get_Item_Type(std::string &db_type) " + db_type + " passthrough error");
      return Item_Type::mainhand;
    }
  }


  std::string Get_Item_Type_String(Item_Type &db_type) {
    if (db_type == Item_Type::back) {
      return "back";
    } else if (db_type == Item_Type::mainhand) {
      return "mainhand";
    } else if (db_type == Item_Type::amulet) {
      return "amulet";
    } else if (db_type == Item_Type::face) {
      return "face";
    } else if (db_type == Item_Type::facialHair) {
      return "facialHair";
    } else if (db_type == Item_Type::belt) {
      return "belt";
    } else if (db_type == Item_Type::boots) {
      return "boots";
    } else if (db_type == Item_Type::ranged) {
      return "ranged";
    } else if (db_type == Item_Type::chest) {
      return "chest";
    } else if (db_type == Item_Type::clothes) {
      return "clothes";
    } else if (db_type == Item_Type::crown) {
      return "crown";
    } else if (db_type == Item_Type::dirt) {
      return "dirt";
    } else if (db_type == Item_Type::gloves) {
      return "gloves";
    } else if (db_type == Item_Type::hair) {
      return "hair";
    } else if (db_type == Item_Type::helm) {
      return "helm";
    } else if (db_type == Item_Type::hood) {
      return "hood";
    } else if (db_type == Item_Type::kilt) {
      return "kilt";
    } else if (db_type == Item_Type::legs) {
      return "legs";
    } else if (db_type == Item_Type::ring) {
      return "ring";
    } else if (db_type == Item_Type::offhand) {
      return "offhand";
    } else if (db_type == Item_Type::shins) {
      return "shins";
    } else if (db_type == Item_Type::shoulders) {
      return "shoulders";
    } else if (db_type == Item_Type::wrist) {
      return "wrist";
    } else if (db_type == Item_Type::jewelry) {
      return "jewelry";
    } else if (db_type == Item_Type::horns) {
      return "horns";
    } else {
      Utilities::Log("Get_Item_Type(std::string &db_type) " + std::to_string((int)db_type) + " passthrough error");
      return "mainhand";
    }
  }

  enum class Unit_Equip_Type {
    Medieval_Underdeep_Dwarves_Male,
    RTP_female,
    RTP_male,
    classes_female,
    classes_male,
    goblin,
    medieval_human_female,
    medieval_human_male,
    zombie,
    none,
    size
  };

  Unit_Equip_Type Get_Unit_Equip_Type(std::string &db_type) {
    if (db_type == "Medieval_Underdeep_Dwarves_Male") {
      return Unit_Equip_Type::Medieval_Underdeep_Dwarves_Male;
    } else if (db_type == "RTP_female") {
      return Unit_Equip_Type::RTP_female;
    } else if (db_type == "RTP_male") {
      return Unit_Equip_Type::RTP_male;
    } else if (db_type == "classes_female") {
      return Unit_Equip_Type::classes_female;
    } else if (db_type == "classes_male") {
      return Unit_Equip_Type::classes_male;
    } else if (db_type == "goblin") {
      return Unit_Equip_Type::goblin;
    } else if (db_type == "medieval_human_female") {
      return Unit_Equip_Type::medieval_human_female;
    } else if (db_type == "medieval_human_male") {
      return Unit_Equip_Type::medieval_human_male;
    } else if (db_type == "zombie") {
      return Unit_Equip_Type::zombie;
    } else {
      return Unit_Equip_Type::none;
    }
  }

  std::string Get_Unit_Equip_Type_String(Unit_Equip_Type &enum_type) {
    if (enum_type == Unit_Equip_Type::Medieval_Underdeep_Dwarves_Male) {
      return "Medieval_Underdeep_Dwarves_Male";
    } else if (enum_type == Unit_Equip_Type::RTP_female) {
      return "RTP_female";
    } else if (enum_type == Unit_Equip_Type::RTP_male) {
      return "RTP_male";
    } else if (enum_type == Unit_Equip_Type::classes_female) {
      return "classes_female";
    } else if (enum_type == Unit_Equip_Type::classes_male) {
      return "classes_male";
    } else if (enum_type == Unit_Equip_Type::goblin) {
      return "goblin";
    } else if (enum_type == Unit_Equip_Type::medieval_human_female) {
      return "medieval_human_female";
    } else if (enum_type == Unit_Equip_Type::medieval_human_male) {
      return "medieval_human_male";
    } else if (enum_type == Unit_Equip_Type::zombie) {
      return "zombie";
    } else {
      return "none";
    }
  }

  struct Item_Pickup {
  };

  struct Equip_Items_Type {
  };

  std::vector<entt::entity> emptyEquipSlot(World::numZones);

  struct Equipment {
    Unit_Equip_Type type = Unit_Equip_Type::none;
    std::map<Item_Type, entt::entity> equippedItems;
  };

  Equipment Emplace_Equipment(entt::registry &zone, int &state, entt::entity &entity, Unit_Equip_Type &itemType) {
    auto &equipment = zone.emplace_or_replace<Equipment>(entity);
    equipment.type = itemType;
    //    std::cout << "equipment slot entity: " << (int)emptyEquipSlot[state] << std::endl;
    equipment.equippedItems = {{Item_Type::amulet, emptyEquipSlot[state]},
                               {Item_Type::back, emptyEquipSlot[state]},
                               {Item_Type::face, emptyEquipSlot[state]},
                               {Item_Type::belt, emptyEquipSlot[state]},
                               {Item_Type::wrist, emptyEquipSlot[state]},
                               {Item_Type::boots, emptyEquipSlot[state]},
                               {Item_Type::dirt, emptyEquipSlot[state]},
                               {Item_Type::hood, emptyEquipSlot[state]},
                               {Item_Type::helm, emptyEquipSlot[state]},
                               {Item_Type::legs, emptyEquipSlot[state]},
                               {Item_Type::kilt, emptyEquipSlot[state]},
                               {Item_Type::gloves, emptyEquipSlot[state]},
                               {Item_Type::clothes, emptyEquipSlot[state]},
                               {Item_Type::chest, emptyEquipSlot[state]},
                               {Item_Type::hair, emptyEquipSlot[state]},
                               {Item_Type::crown, emptyEquipSlot[state]},
                               {Item_Type::ring, emptyEquipSlot[state]},
                               {Item_Type::shins, emptyEquipSlot[state]},
                               {Item_Type::shoulders, emptyEquipSlot[state]},
                               {Item_Type::jewelry, emptyEquipSlot[state]},
                               {Item_Type::ranged, emptyEquipSlot[state]},
                               {Item_Type::offhand, emptyEquipSlot[state]},
                               {Item_Type::mainhand, emptyEquipSlot[state]}};
    //    for (auto item :equipment.equippedItems) {
    //      Utilities::Log((int)item.second);
    //    }
    return equipment;
  }

  struct Item {
    std::string name;
    std::string face_pngPath;
    std::string body_pngPath;
    Unit_Equip_Type equip_type;
  };

  enum class Armor_Type {
    cloth,
    padded,
    leather,
    mail,
    plate
  };

  enum class Weapon_Type {
    sword,
    axe,
    mace,
    spear
  };

  enum class Offhand_Type {
    dagger,
    kama,
    shield
  };

  enum class Weapon_Material {
    copper,
    bronze,
    iron,
  };

  struct Ground_Item {
    SDL_FRect box = {};
    SDL_FRect ground_name = {};
  };

  struct Update_Ground_Item {
  };

  struct Name {
    std::string name;
  };

  enum class Rarity {
    common,//white
    magic, //blue
    rare,  //yellow
    unique //gold
  };

  struct Item_Equip {
  };

  struct Weapon_Damage {
    int min;
    int max;
  };


  struct statRange {
    int min;
    int max;
  };

  //This is the order that it will show the stats on the item
  enum class Stat {
    armor,
    damage,
    spellDamage,
    health,
    piety,
    attackSpeed
  };

  struct statValue {
    Stat name;
    int value;
  };

  struct Item_Stats {
    ;
    std::map<Stat, int> stats = {

    };
  };

  std::unordered_map<Rarity, SDL_Color> rarityColor;
  std::unordered_map<Rarity, SDL_Texture *> rarityBorder;
  std::unordered_map<Stat, int> baseStatData;
  std::unordered_map<Stat, int> statData;
  std::unordered_map<Stat, std::string> statName;
  std::unordered_map<Item_Type, int> itemTypes;
  std::unordered_map<Armor_Type, int> armorTypes;
  std::unordered_map<Weapon_Type, int> weaponTypes;
  std::unordered_map<Offhand_Type, int> offhandTypes;
  std::unordered_map<Weapon_Material, int> weaponMaterials;
  std::unordered_map<Weapon_Material, std::string> weaponMaterialName;
  std::unordered_map<Weapon_Type, std::string> weaponTypeName;
  std::unordered_map<Offhand_Type, std::string> offhandTypeName;
  std::unordered_map<Armor_Type, std::string> ArmorTypeName;
  std::unordered_map<Item_Type, std::string> ItemTypeName;
}// namespace Item_Component
