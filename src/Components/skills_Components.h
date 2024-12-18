//
// Created by nathanial on 12/17/24.
//
#pragma once
#include "array"
#include "string"

namespace  Skill_Component {

    struct Skills_Points {
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
        zone.emplace_or_replace<Skills_Points>(entity);
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

}