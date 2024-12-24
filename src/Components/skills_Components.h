//
// Created by nathanial on 12/17/24.
//
#pragma once

#include "array"
#include "string"

namespace Skill_Component {

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

    //Warrior Skill

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

    struct Chopping {
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

    void Add_Default_Warrior(entt::registry &zone, entt::entity &entity) {
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

    void Copy_Warrior_Skills(entt::registry &newZone, entt::registry &oldZone, entt::entity newEntity, entt::entity oldEntity) {
	newZone.emplace_or_replace<Armour_Smithing>(newEntity) = oldZone.get<Armour_Smithing>(oldEntity);
	newZone.emplace_or_replace<Armour_Use>(newEntity) = oldZone.get<Armour_Use>(oldEntity);
	newZone.emplace_or_replace<Blocking>(newEntity) = oldZone.get<Blocking>(oldEntity);
	newZone.emplace_or_replace<Bludgeoning>(newEntity) = oldZone.get<Bludgeoning>(oldEntity);
	newZone.emplace_or_replace<Chopping>(newEntity) = oldZone.get<Chopping>(oldEntity);
	newZone.emplace_or_replace<Combat>(newEntity) = oldZone.get<Combat>(oldEntity);
	newZone.emplace_or_replace<Crusade>(newEntity) = oldZone.get<Crusade>(oldEntity);
	newZone.emplace_or_replace<Disease_Resistance>(newEntity) = oldZone.get<Disease_Resistance>(oldEntity);
	newZone.emplace_or_replace<Dragon_Slaying>(newEntity) = oldZone.get<Dragon_Slaying>(oldEntity);
	newZone.emplace_or_replace<Dual_Wielding>(newEntity) = oldZone.get<Dual_Wielding>(oldEntity);
	newZone.emplace_or_replace<Physical_Resistance>(newEntity) = oldZone.get<Physical_Resistance>(oldEntity);
	newZone.emplace_or_replace<Repair>(newEntity) = oldZone.get<Repair>(oldEntity);
	newZone.emplace_or_replace<Spearing>(newEntity) = oldZone.get<Spearing>(oldEntity);
	newZone.emplace_or_replace<Swordplay>(newEntity) = oldZone.get<Swordplay>(oldEntity);
	newZone.emplace_or_replace<Titanology>(newEntity) = oldZone.get<Titanology>(oldEntity);
	newZone.emplace_or_replace<Unarmed_Fighting>(newEntity) = oldZone.get<Unarmed_Fighting>(oldEntity);
	newZone.emplace_or_replace<Violence>(newEntity) = oldZone.get<Violence>(oldEntity);
	newZone.emplace_or_replace<Weapon_Smithing>(newEntity) = oldZone.get<Weapon_Smithing>(oldEntity);
	newZone.emplace_or_replace<Weapon_Use>(newEntity) = oldZone.get<Weapon_Use>(oldEntity);
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

    void Add_Default_General(entt::registry &zone, entt::entity &entity) {
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

    void Copy_General_Skills(entt::registry &newZone, entt::registry &oldZone, entt::entity newEntity, entt::entity oldEntity) {
	newZone.emplace_or_replace<Bartering>(newEntity) = oldZone.get<Bartering>(oldEntity);
	newZone.emplace_or_replace<Cartography>(newEntity) = oldZone.get<Cartography>(oldEntity);
	newZone.emplace_or_replace<Cold_Resistance>(newEntity) = oldZone.get<Cold_Resistance>(oldEntity);
	newZone.emplace_or_replace<Cooking>(newEntity) = oldZone.get<Cooking>(oldEntity);
	newZone.emplace_or_replace<Death_Cheating>(newEntity) = oldZone.get<Death_Cheating>(oldEntity);
	newZone.emplace_or_replace<Electrical_Resistance>(newEntity) = oldZone.get<Electrical_Resistance>(oldEntity);
	newZone.emplace_or_replace<Fitness>(newEntity) = oldZone.get<Fitness>(oldEntity);
	newZone.emplace_or_replace<Gemcutting>(newEntity) = oldZone.get<Gemcutting>(oldEntity);
	newZone.emplace_or_replace<Magic_Item_Use>(newEntity) = oldZone.get<Magic_Item_Use>(oldEntity);
	newZone.emplace_or_replace<Mining>(newEntity) = oldZone.get<Mining>(oldEntity);
	newZone.emplace_or_replace<Piety>(newEntity) = oldZone.get<Piety>(oldEntity);
	newZone.emplace_or_replace<Swimming>(newEntity) = oldZone.get<Swimming>(oldEntity);
	newZone.emplace_or_replace<Self_Awareness>(newEntity) = oldZone.get<Self_Awareness>(oldEntity);
	newZone.emplace_or_replace<Traveling>(newEntity) = oldZone.get<Traveling>(oldEntity);
	newZone.emplace_or_replace<Willpower>(newEntity) = oldZone.get<Willpower>(oldEntity);
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
	int level = 2;
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

    void Add_Default_Mage(entt::registry &zone, entt::entity &entity) {
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

    void Copy_Mage_Skills(entt::registry &newZone, entt::registry &oldZone, entt::entity newEntity, entt::entity oldEntity) {
	newZone.emplace_or_replace<Alchemy>(newEntity) = oldZone.get<Alchemy>(oldEntity);
	newZone.emplace_or_replace<Demonology>(newEntity) = oldZone.get<Demonology>(oldEntity);
	newZone.emplace_or_replace<Heat_Resistance>(newEntity) = oldZone.get<Heat_Resistance>(oldEntity);
	newZone.emplace_or_replace<History>(newEntity) = oldZone.get<History>(oldEntity);
	newZone.emplace_or_replace<Magic_Resistance>(newEntity) = oldZone.get<Magic_Resistance>(oldEntity);
	newZone.emplace_or_replace<Memory>(newEntity) = oldZone.get<Memory>(oldEntity);
	newZone.emplace_or_replace<Mutology>(newEntity) = oldZone.get<Mutology>(oldEntity);
	newZone.emplace_or_replace<Necrology>(newEntity) = oldZone.get<Necrology>(oldEntity);
	newZone.emplace_or_replace<Research>(newEntity) = oldZone.get<Research>(oldEntity);
	newZone.emplace_or_replace<Runic_Lore>(newEntity) = oldZone.get<Runic_Lore>(oldEntity);
	newZone.emplace_or_replace<Spell_Use>(newEntity) = oldZone.get<Spell_Use>(oldEntity);
	newZone.emplace_or_replace<Thaumaturgy>(newEntity) = oldZone.get<Thaumaturgy>(oldEntity);
	newZone.emplace_or_replace<Wand_Lore>(newEntity) = oldZone.get<Wand_Lore>(oldEntity);
    }

    ///////////////////////////////////////

    //Rogue Skill

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

    void Add_Default_Rogue(entt::registry &zone, entt::entity &entity) {
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

    void Copy_Rogue_Skills(entt::registry &newZone, entt::registry &oldZone, entt::entity newEntity, entt::entity oldEntity) {
	newZone.emplace_or_replace<Acid_Resistance>(newEntity) = oldZone.get<Acid_Resistance>(oldEntity);
	newZone.emplace_or_replace<Archery>(newEntity) = oldZone.get<Archery>(oldEntity);
	newZone.emplace_or_replace<Assassination>(newEntity) = oldZone.get<Assassination>(oldEntity);
	newZone.emplace_or_replace<Disarming>(newEntity) = oldZone.get<Disarming>(oldEntity);
	newZone.emplace_or_replace<Dodge>(newEntity) = oldZone.get<Dodge>(oldEntity);
	newZone.emplace_or_replace<Entomology>(newEntity) = oldZone.get<Entomology>(oldEntity);
	newZone.emplace_or_replace<Gambling>(newEntity) = oldZone.get<Gambling>(oldEntity);
	newZone.emplace_or_replace<Geography>(newEntity) = oldZone.get<Geography>(oldEntity);
	newZone.emplace_or_replace<Hunting>(newEntity) = oldZone.get<Hunting>(oldEntity);
	newZone.emplace_or_replace<Lock_Picking>(newEntity) = oldZone.get<Lock_Picking>(oldEntity);
	newZone.emplace_or_replace<Pocket_Picking>(newEntity) = oldZone.get<Pocket_Picking>(oldEntity);
	newZone.emplace_or_replace<Poison_Resistance>(newEntity) = oldZone.get<Poison_Resistance>(oldEntity);
	newZone.emplace_or_replace<Scavenging>(newEntity) = oldZone.get<Scavenging>(oldEntity);
	newZone.emplace_or_replace<Secret_Detection>(newEntity) = oldZone.get<Secret_Detection>(oldEntity);
	newZone.emplace_or_replace<Stealth>(newEntity) = oldZone.get<Stealth>(oldEntity);
	newZone.emplace_or_replace<Trap_Setting>(newEntity) = oldZone.get<Trap_Setting>(oldEntity);
	newZone.emplace_or_replace<Tumbling>(newEntity) = oldZone.get<Tumbling>(oldEntity);
    }

    ////////////////////////////////////////////

    void Add_Default(entt::registry &zone, entt::entity &entity) {
	zone.emplace_or_replace<Skills_Points>(entity);
	Add_Default_Warrior(zone, entity);
	Add_Default_General(zone, entity);
	Add_Default_Mage(zone, entity);
	Add_Default_Rogue(zone, entity);
    }

    void Copy_Skills(entt::registry &newZone, entt::registry &oldZone, entt::entity newEntity, entt::entity oldEntity) {
	newZone.emplace_or_replace<Skills_Points>(newEntity) = oldZone.get<Skills_Points>(oldEntity);
	Copy_Warrior_Skills(newZone, oldZone, newEntity, oldEntity);
	Copy_General_Skills(newZone, oldZone, newEntity, oldEntity);
	Copy_Mage_Skills(newZone, oldZone, newEntity, oldEntity);
	Copy_Rogue_Skills(newZone, oldZone, newEntity, oldEntity);
    }
}