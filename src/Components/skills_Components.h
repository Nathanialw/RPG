//
// Created by nathanial on 12/17/24.
//
#pragma once
#include "array"
#include "string"

namespace  Skill_Component {

    struct Skills_Points {
        int bankedFromInt = 0;
        int unspent = 0;

        int generalCost = 1;
        int generalMax = 2;
        int mageCost = 1;
        int mageMax = 2;
        int rogueCost = 2;
        int rogueMax = 2;
        int warriorCost = 3;
        int warriorMax = 2;
    };

    //Warrior Skills

    struct Armour_Smithing {
        int level = 0;
        int maxLevel = 3;
    };

    struct Armour_Use {
        int level = 1;
        int maxLevel = 1;
    };

    struct Blocking {
        int level = 0;
        int maxLevel = 3;
    };

    struct Bludgeoning {
        int level = 0;
        int maxLevel = 3;
    };

    struct Chopping  {
        int level = 0;
        int maxLevel = 3;
    };

    struct Combat {
        int level = 0;
        int maxLevel = 3;
    };

    struct Crusade {
        int level = 0;
        int maxLevel = 3;
    };

    struct Disease_Resistance {
        int level = 0;
        int maxLevel = 3;
    };

    struct Dragon_Slaying {
        int level = 0;
        int maxLevel = 3;
    };

    struct Dual_Wielding {
        int level = 0;
        int maxLevel = 3;
    };

    struct Physical_Resistance {
        int level = 0;
        int maxLevel = 3;
    };

    struct Repair {
        int level = 0;
        int maxLevel = 3;
    };

    struct Spearing {
        int level = 0;
        int maxLevel = 3;
    };

    struct Swordplay {
        int level = 0;
        int maxLevel = 3;
    };

    struct Titanology {
        int level = 0;
        int maxLevel = 3;
    };

    struct Unarmed_Fighting {
        int level = 0;
        int maxLevel = 3;
    };

    struct Violence {
        int level = 0;
        int maxLevel = 3;
    };

    struct Weapon_Smithing {
        int level = 0;
        int maxLevel = 3;
    };

    struct Weapon_Use {
        int level = 0;
        int maxLevel = 3;
    };

    void Add_Default_Warrior(entt::registry  &zone, entt::entity &entity) {
        zone.emplace_or_replace<Armour_Smithing>(entity);
        zone.emplace_or_replace<Armour_Use>(entity);
        zone.emplace_or_replace<Blocking>(entity);
        zone.emplace_or_replace<Bludgeoning>(entity);
        zone.emplace_or_replace<Chopping>(entity);
        zone.emplace_or_replace<Combat>(entity);
        zone.emplace_or_replace<Crusade>(entity);
        zone.emplace_or_replace<Disease_Resistance>(entity);
        zone.emplace_or_replace<Dragon_Slaying>(entity);
        zone.emplace_or_replace<Dual_Wielding>(entity);
        zone.emplace_or_replace<Physical_Resistance>(entity);
        zone.emplace_or_replace<Repair>(entity);
        zone.emplace_or_replace<Spearing>(entity);
        zone.emplace_or_replace<Swordplay>(entity);
        zone.emplace_or_replace<Titanology>(entity);
        zone.emplace_or_replace<Unarmed_Fighting>(entity);
        zone.emplace_or_replace<Violence>(entity);
        zone.emplace_or_replace<Weapon_Smithing>(entity);
        zone.emplace_or_replace<Weapon_Use>(entity);
    }

    ////////////////////////////////

    //General skills

    struct Bartering {
        int level = 0;
        int maxLevel = 3;
    };

    struct Cartography {
        int level = 0;
        int maxLevel = 3;
    };

    struct Cold_Resistance {
        int level = 0;
        int maxLevel = 3;
    };

    struct Cooking {
        int level = 0;
        int maxLevel = 3;
    };

    struct Death_Cheating {
        int level = 0;
        int maxLevel = 3;
    };

    struct Electrical_Resistance {
        int level = 0;
        int maxLevel = 3;
    };

    struct Fitness {
        int level = 0;
        int maxLevel = 3;
    };

    struct Gemcutting {
        int level = 0;
        int maxLevel = 3;
    };

    struct Magic_Item_Use {
        int level = 0;
        int maxLevel = 3;
    };

    struct Mining {
        int level = 0;
        int maxLevel = 3;
    };

    struct Piety {
        int level = 0;
        int maxLevel = 3;
    };

    struct Swimming {
        int level = 0;
        int maxLevel = 3;
    };

    struct Self_Awareness {
        int level = 0;
        int maxLevel = 3;
    };

    struct Traveling {
        int level = 0;
        int maxLevel = 3;
    };

    struct Willpower {
        int level = 0;
        int maxLevel = 3;
    };

    void Add_Default_General(entt::registry  &zone, entt::entity &entity) {
        zone.emplace_or_replace<Bartering>(entity);
        zone.emplace_or_replace<Cartography>(entity);
        zone.emplace_or_replace<Cold_Resistance>(entity);
        zone.emplace_or_replace<Cooking>(entity);
        zone.emplace_or_replace<Death_Cheating>(entity);
        zone.emplace_or_replace<Electrical_Resistance>(entity);
        zone.emplace_or_replace<Fitness>(entity);
        zone.emplace_or_replace<Gemcutting>(entity);
        zone.emplace_or_replace<Magic_Item_Use>(entity);
        zone.emplace_or_replace<Mining>(entity);
        zone.emplace_or_replace<Piety>(entity);
        zone.emplace_or_replace<Swimming>(entity);
        zone.emplace_or_replace<Self_Awareness>(entity);
        zone.emplace_or_replace<Traveling>(entity);
        zone.emplace_or_replace<Willpower>(entity);
    }

    ////////////////////////////////////////////

    //mage skills

    struct Alchemy {
        int level = 0;
        int maxLevel = 3;
    };

    struct Demonology {
        int level = 0;
        int maxLevel = 3;
    };

    struct Heat_Resistance {
        int level = 0;
        int maxLevel = 3;
    };

    struct History {
        int level = 0;
        int maxLevel = 3;
    };

    struct Magic_Resistance {
        int level = 0;
        int maxLevel = 3;
    };

    struct Memory {
        int level = 0;
        int maxLevel = 3;
    };

    struct Mutology {
        int level = 0;
        int maxLevel = 3;
    };

    struct Necrology {
        int level = 0;
        int maxLevel = 3;
    };

    struct Research {
        int level = 0;
        int maxLevel = 3;
    };

    struct Runic_Lore {
        int level = 0;
        int maxLevel = 3;
    };

    struct Spell_Use {
        int level = 0;
        int maxLevel = 3;
    };

    struct Thaumaturgy {
        int level = 0;
        int maxLevel = 3;
    };

    struct Wand_Lore {
        int level = 0;
        int maxLevel = 3;
    };

    void Add_Default_Mage(entt::registry  &zone, entt::entity &entity) {
        zone.emplace_or_replace<Alchemy>(entity);
        zone.emplace_or_replace<Demonology>(entity);
        zone.emplace_or_replace<Heat_Resistance>(entity);
        zone.emplace_or_replace<History>(entity);
        zone.emplace_or_replace<Magic_Resistance>(entity);
        zone.emplace_or_replace<Memory>(entity);
        zone.emplace_or_replace<Mutology>(entity);
        zone.emplace_or_replace<Necrology>(entity);
        zone.emplace_or_replace<Research>(entity);
        zone.emplace_or_replace<Runic_Lore>(entity);
        zone.emplace_or_replace<Spell_Use>(entity);
        zone.emplace_or_replace<Thaumaturgy>(entity);
        zone.emplace_or_replace<Wand_Lore>(entity);
    }


    ///////////////////////////////////////

    //Rogue Skills

    struct Acid_Resistance {
        int level = 0;
        int maxLevel = 3;
    };

    struct Archery {
        int level = 0;
        int maxLevel = 3;
    };

    struct Assassination {
        int level = 0;
        int maxLevel = 3;
    };

    struct Disarming {
        int level = 0;
        int maxLevel = 3;
    };

    struct Dodge {
        int level = 0;
        int maxLevel = 3;
    };

    struct Entomology {
        int level = 0;
        int maxLevel = 3;
    };

    struct Gambling {
        int level = 0;
        int maxLevel = 3;
    };

    struct Geography {
        int level = 0;
        int maxLevel = 3;
    };

    struct Hunting {
        int level = 0;
        int maxLevel = 3;
    };

    struct Lock_Picking {
        int level = 0;
        int maxLevel = 3;
    };

    struct Pocket_Picking {
        int level = 0;
        int maxLevel = 3;
    };

    struct Poison_Resistance {
        int level = 0;
        int maxLevel = 3;
    };

    struct Scavenging {
        int level = 0;
        int maxLevel = 3;
    };

    struct Secret_Detection {
        int level = 0;
        int maxLevel = 3;
    };

    struct Stealth {
        int level = 0;
        int maxLevel = 3;
    };

    struct Trap_Setting {
        int level = 0;
        int maxLevel = 3;
    };

    struct Tumbling {
        int level = 0;
        int maxLevel = 3;
    };

    void Add_Default_Rogue(entt::registry  &zone, entt::entity &entity) {
        zone.emplace_or_replace<Acid_Resistance>(entity);
        zone.emplace_or_replace<Archery>(entity);
        zone.emplace_or_replace<Assassination>(entity);
        zone.emplace_or_replace<Disarming>(entity);
        zone.emplace_or_replace<Dodge>(entity);
        zone.emplace_or_replace<Entomology>(entity);
        zone.emplace_or_replace<Gambling>(entity);
        zone.emplace_or_replace<Geography>(entity);
        zone.emplace_or_replace<Hunting>(entity);
        zone.emplace_or_replace<Lock_Picking>(entity);
        zone.emplace_or_replace<Pocket_Picking>(entity);
        zone.emplace_or_replace<Poison_Resistance>(entity);
        zone.emplace_or_replace<Scavenging>(entity);
        zone.emplace_or_replace<Secret_Detection>(entity);
        zone.emplace_or_replace<Stealth>(entity);
        zone.emplace_or_replace<Trap_Setting>(entity);
        zone.emplace_or_replace<Tumbling>(entity);
    }

    ////////////////////////////////////////////

    void Add_Default(entt::registry  &zone, entt::entity &entity) {
        zone.emplace_or_replace<Skills_Points>(entity);
        Add_Default_Warrior(zone, entity);
        Add_Default_General(zone, entity);
        Add_Default_Mage(zone, entity);
        Add_Default_Rogue(zone, entity);
    }
}