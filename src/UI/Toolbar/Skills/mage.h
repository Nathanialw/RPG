//
// Created by nathanial on 12/17/24.
//
#pragma once
#include "skills.h"

//passive skills
namespace  Skills {
    namespace Mage {
        constexpr int SIZE = 13;
        typedef bool (*Action)();

        std::array<std::pair<int , int>, SIZE> Get_Skills(entt::registry &zone) {
            std::array<std::pair<int , int>, SIZE> currentSkills = {};

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
            }
            return currentSkills;
        }

        Skill_Values Get_Skill_Points(entt::registry &zone) {
            auto view = zone.view<Component::Input, Skill_Component::Skills_Points>();
            for (auto entity: view) {
                auto skillPoints = view.get<Skill_Component::Skills_Points>(entity);
                return {skillPoints.unspent, skillPoints.mageCost, skillPoints.mageMax};
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
        };

        Skill_Tree<SIZE, Action> mage = Skill_Tree<SIZE, Action>(names,  icons, "Mage");


        void Update(entt::registry  &zone, f2 scale) {
            mage.Update(scale, Get_Skills(zone), Get_Skill_Points(zone));
        }

        void Render() {
            mage.Draw();
        }

        bool Toggle() {
            return mage.Toggle();
        }

        void Close() {
            mage.Close();
        }
    }
}