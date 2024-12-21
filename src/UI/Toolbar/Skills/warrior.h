//
// Created by nathanial on 12/17/24.
//
#pragma once
#include "skills.h"

//passive skills
namespace Skill {
    namespace Warrior {
        constexpr int SIZE = 19;

        typedef bool (*Action)();


        bool PLACEHOLDER() {
            return false;
        }

        std::array<std::pair<int, int>, SIZE> Get_Skills(entt::registry &zone) {
            std::array<std::pair<int, int>, SIZE> currentSkills = {};

            auto view = zone.view<Component::Input, Skill_Component::Skills_Points,
                    Skill_Component::Armour_Smithing,
                    Skill_Component::Armour_Use,
                    Skill_Component::Blocking,
                    Skill_Component::Bludgeoning,
                    Skill_Component::Chopping,
                    Skill_Component::Combat,
                    Skill_Component::Crusade,
                    Skill_Component::Disease_Resistance,
                    Skill_Component::Dragon_Slaying,
                    Skill_Component::Dual_Wielding,
                    Skill_Component::Physical_Resistance,
                    Skill_Component::Repair,
                    Skill_Component::Spearing,
                    Skill_Component::Swordplay,
                    Skill_Component::Titanology,
                    Skill_Component::Unarmed_Fighting,
                    Skill_Component::Violence,
                    Skill_Component::Weapon_Smithing,
                    Skill_Component::Weapon_Use>();

            for (auto entity: view) {
                auto &armourSmithing = view.get<Skill_Component::Armour_Smithing>(entity);
                auto &armourUse = view.get<Skill_Component::Armour_Use>(entity);
                auto &blocking = view.get<Skill_Component::Blocking>(entity);
                auto &bludgeoning = view.get<Skill_Component::Bludgeoning>(entity);
                auto &chopping = view.get<Skill_Component::Chopping>(entity);
                auto &combat = view.get<Skill_Component::Combat>(entity);
                auto &crusade = view.get<Skill_Component::Crusade>(entity);
                auto &diseaseResistance = view.get<Skill_Component::Disease_Resistance>(entity);
                auto &dragonSlaying = view.get<Skill_Component::Dragon_Slaying>(entity);
                auto &dualWielding = view.get<Skill_Component::Dual_Wielding>(entity);
                auto &physicalResistance = view.get<Skill_Component::Physical_Resistance>(entity);
                auto &repair = view.get<Skill_Component::Repair>(entity);
                auto &spearing = view.get<Skill_Component::Spearing>(entity);
                auto &swordplay = view.get<Skill_Component::Swordplay>(entity);
                auto &titanology = view.get<Skill_Component::Titanology>(entity);
                auto &unarmedFighting = view.get<Skill_Component::Unarmed_Fighting>(entity);
                auto &violence = view.get<Skill_Component::Violence>(entity);
                auto &weaponSmithing = view.get<Skill_Component::Weapon_Smithing>(entity);
                auto &weaponUse = view.get<Skill_Component::Weapon_Use>(entity);
                currentSkills[0] = {armourSmithing.level, armourSmithing.maxLevel};
                currentSkills[1] = {armourUse.level, armourUse.maxLevel};
                currentSkills[2] = {blocking.level, blocking.maxLevel};
                currentSkills[3] = {bludgeoning.level, bludgeoning.maxLevel};
                currentSkills[4] = {chopping.level, chopping.maxLevel};
                currentSkills[5] = {combat.level, combat.maxLevel};
                currentSkills[6] = {crusade.level, crusade.maxLevel};
                currentSkills[7] = {diseaseResistance.level, diseaseResistance.maxLevel};
                currentSkills[8] = {dragonSlaying.level, dragonSlaying.maxLevel};
                currentSkills[9] = {dualWielding.level, dualWielding.maxLevel};
                currentSkills[10] = {physicalResistance.level, physicalResistance.maxLevel};
                currentSkills[11] = {repair.level, repair.maxLevel};
                currentSkills[12] = {spearing.level, spearing.maxLevel};
                currentSkills[13] = {swordplay.level, swordplay.maxLevel};
                currentSkills[14] = {titanology.level, titanology.maxLevel};
                currentSkills[15] = {unarmedFighting.level, unarmedFighting.maxLevel};
                currentSkills[16] = {violence.level, violence.maxLevel};
                currentSkills[17] = {weaponSmithing.level, weaponSmithing.maxLevel};
                currentSkills[18] = {weaponUse.level, weaponUse.maxLevel};
            }
            return currentSkills;
        }

        Skill_Values Get_Skill_Points(entt::registry &zone) {
            auto view = zone.view<Component::Input, Skill_Component::Skills_Points>();
            for (auto entity: view) {
                auto skillPoints = view.get<Skill_Component::Skills_Points>(entity);
                return {skillPoints.unspent, skillPoints.warriorCost, skillPoints.warriorMax};
            }
        }

        std::array<std::string, SIZE> names = {
               "Armour Smithing",
               "Armour Use",
               "Blocking",
               "Bludgeoning",
               "Chopping",
               "Combat",
               "Crusade",
               "Disease Resistance",
               "Dragon Slaying",
               "Dual Wielding",
               "Physical Resistance",
               "Repair",
               "Spearing",
               "Swordplay",
               "Titanology",
               "Unarmed Fighting",
               "Violence",
               "Weapon Smithing",
               "Weapon Use"
       };

        std::array<std::string, SIZE> icons = {
                "armoursmithingicon",
                "armouruseicon",
                "blockingicon",
                "bludgeoningicon",
                "choppingicon",
                "combaticon",
                "crusadeicon",
                "diseaseresistanceicon",
                "dragonslayingicon",
                "dualwieldingicon",
                "physicalresistanceicon",
                "repairicon",
                "spearingicon",
                "swordplayicon",
                "titanologyicon",
                "unarmedfightingicon",
                "violenceicon",
                "weaponsmithingicon",
                "weaponuseicon"
        };

        std::array<std::array<std::string, 5>, SIZE> descriptions = {
                {{" has no effect.", " grants a 25%% chance to create a new non-magical Armour with each new Dungeon map you enter.", " grants a 50%% chance, instead.", " grants a 75%% chance, instead.", "The type of Armour created will be randomly chosen but will correspond in strength to the Dungeon Level you've entered."},
                 {" has no effect.", " allows you to equip Light Armour", " allows you to equip Medium Armour", " allows you to equip Heavy Armour", ""},
                 {" has no effect.", " increases the Defence bonus of Shields you use by 50%%.", " increases the bonus by 100%%, instead.", " increases the bonus by 150%%, instead.", ""},
                 {" has no effect.", " adds 3 to the damage you do while using any Blunt Weapon.", " also gives you +35 to hit while using any Blunt Weapon.", " also adds 25%% to the weapon damage done by any Blunt Weapon.", "Bludgeoning may also stand in for Weapon Use when equipping Blunt Weapons, so long as you Class can attain an equivalent Weapon Use Skill level."},
                 {" has no effect.", " adds 3 to the damage you do while using any Axe.", " also gives you +35 to hit while using any Axe.", " also adds 25%% to the weapon damage done by any Axe.", "Chopping may also stand in for Weapon Use when equipping Axes, so long as you Class can attain an equivalent Weapon Use Skill level."},
                 {" has no effect.", " gives you +15 to hit.", " gives you +30 to hit, instead.", " gives you +45 to hit, instead.", ""},
                 {" has no effect.", " gives you +30 to hit vs. Goblinoids.", " also grants you a 20%% chance to find a new Weapon, each time you kill a Goblinoid.", " also halves damage you take from Goblinoids in combat.", ""},
                 {" has no effect.", " gives you a 25%% chance to avoid contracting any Disease, and a 15%% chance on entering any new map of losing any Disease you do have.", " gives a 50%% chance to avoid contracting and a 30%% chance to remove any Disease you do have, instead.", " gives a 75%% chance to avoid contracting and a 45%% chance to remove any Disease you do have, instead.", ""},
                 {" has no effect.", " gives you +30 to hit vs. Wyrms.", " also grants you an extra 50 Experience Points each time you kill a Wyrm.", " also halves damage you take from Wyrms in combat.", ""},
                 {" has no effect.", " allows the equipping of a 2nd Weapon; two separate attacks are made, though only the cumulative result is given.  Both attacks have a -30 to hit.", " does likewise, but have only a -20 to hit.", " leaves only -10 to hit for each.", "Dual Wielding prohibits Shields and the use of 2-h Weapons.  Distance Strike is only Possible if both Weapons are Missile."},
                 {" has no effect.", " lowers incoming Physical damage by 10%%.", " lowers it by 20%%, instead.", " lowers it by 30%%, instead.", "Physical damage does not include damage from falling, drowning, suffocating, explosions, Cave Ins or consumption of anything."},
                 {" grants a 25%% chance to restore 25%% Condition to any damaged Weapon or Armour at a Forge (with failure destroying the Forge).", " grants a 50%% chance, instead.", " grants a 75%% chance, instead.", " grants a 90%% chance, instead.", ""},
                 {" has no effect.", " adds 3 to the damage you do while using any Pole Arm.", " also gives you +35 to hit while using any Pole Arm.", " also adds 25%% to the weapon damage done by any Pole Arm.", "Spearing may also stand in for Weapon Use when equipping Pole Arms, so long as you Class can attain an equivalent Weapon Use Skill level."},
                 {" has no effect.", " adds 3 to the damage you do while using any Blade Weapon.", " also gives you +35 to hit while using any Blade Weapon.", " also adds 25%% to the weapon damage done by any Blade Weapon.", "Swordplay may also stand in for Weapon Use when equipping Blade Weapons, so long as you Class can attain an equivalent Weapon Use Skill level."},
                 {" has no effect.", " gives you +30 to hit vs. Giant Types.", " also grants you a 20% chance to find a new Armour, each time you kill a Giant Type.", " also halves damage you take from Giant Types in combat.", ""},
                 {" has no effect.", " adds 1 to 4 to the damage you do when you hit a monster with no Weapon equipped.", " adds 2 to 8, instead.", " adds 3 to 12, instead.", ""},
                 {" has no effect.", " calculates bonus Strength damage as if your Strength was 25%% higher whe n determining how much extra damage you do.", " calculates bonus Strength damage as though your Strength was 50%% higher, instead.", " calculates bonus Strength damage as though your Strength was 75%% higher, instead.", ""},
                 {" has no effect.", " grants a 25%% chance to create a new non-magical Weapon with each new Dungeon map you enter.", " grants a 50%% chance, instead.", " grants a 75%% chance, instead.", "The type of Weapon created will be randomly chosen but will correspond in strength to the Dungeon Level you've entered."},
                 {" has no effect.", " allows you to equip Light Weapons.", " allows you to equip Medium Weapons.", " allows you to equip Heavy Weapons.", ""}}
        };

        Skill_Tree <SIZE, Action> warrior = Skill_Tree<SIZE, Action>(names, icons, descriptions, "Warrior");


        void Update(entt::registry &zone, f2 scale) {
            warrior.Update(scale, Get_Skills(zone), Get_Skill_Points(zone));
        }

        bool Mouse_Inside() {
            return warrior.Mouse_Inside();
        }

        void Hover_Highlight() {
            warrior.Mouse_Over();
        }

        bool Click(entt::registry &zone, entt::entity &entity) {
            //in Event Manager check for click on skill up button
            //get index of skill
            int i = warrior.Click();
            if (i == -1)
                return false;

            auto &skills = zone.get<Skill_Component::Skills_Points>(entity);
            bool clicked = false;

            if (skills.unspent < skills.warriorCost)
                return false;

            switch (i) {
                case 0:     clicked = Increase_Skill(zone.get<Skill_Component::Armour_Smithing>(entity));   break;
                case 1:     clicked = Increase_Skill(zone.get<Skill_Component::Armour_Use>(entity));    break;
                case 2:     clicked = Increase_Skill(zone.get<Skill_Component::Blocking>(entity));  break;
                case 3:     clicked = Increase_Skill(zone.get<Skill_Component::Bludgeoning>(entity));   break;
                case 4:     clicked = Increase_Skill(zone.get<Skill_Component::Chopping>(entity));  break;
                case 5:     clicked = Increase_Skill(zone.get<Skill_Component::Combat>(entity));    break;
                case 6:     clicked = Increase_Skill(zone.get<Skill_Component::Crusade>(entity));   break;
                case 7:     clicked = Increase_Skill(zone.get<Skill_Component::Disease_Resistance>(entity)); break;
                case 8:     clicked = Increase_Skill(zone.get<Skill_Component::Dragon_Slaying>(entity));    break;
                case 9:     clicked = Increase_Skill(zone.get<Skill_Component::Dual_Wielding>(entity)); break;
                case 10:    clicked = Increase_Skill(zone.get<Skill_Component::Physical_Resistance>(entity)); break;
                case 11:    clicked = Increase_Skill(zone.get<Skill_Component::Repair>(entity));    break;
                case 12:    clicked = Increase_Skill(zone.get<Skill_Component::Spearing>(entity));  break;
                case 13:    clicked = Increase_Skill(zone.get<Skill_Component::Swordplay>(entity)); break;
                case 14:    clicked = Increase_Skill(zone.get<Skill_Component::Titanology>(entity));    break;
                case 15:    clicked = Increase_Skill(zone.get<Skill_Component::Unarmed_Fighting>(entity));  break;
                case 16:    clicked = Increase_Skill(zone.get<Skill_Component::Violence>(entity));  break;
                case 17:    clicked = Increase_Skill(zone.get<Skill_Component::Weapon_Smithing>(entity));   break;
                case 18:    clicked = Increase_Skill(zone.get<Skill_Component::Weapon_Use>(entity));    break;
                default:    break;
            }

            if (clicked)
                skills.unspent -= skills.warriorCost;

            return clicked;
        };

        void Render(f2 scale) {
            warrior.Draw(scale);
        }

        bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
            return warrior.Toggle(toggleType);
        }

        void Close() {
            return warrior.Close();
        }
    }
}
