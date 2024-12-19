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

        Skill_Tree <SIZE, Action> warrior = Skill_Tree<SIZE, Action>(names, icons, "Warrior");


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
            }

            if (clicked)
                skills.unspent -= skills.warriorCost;

            return clicked;
        };

        void Render() {
            warrior.Draw();
        }

        bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
            return warrior.Toggle(toggleType);
        }

        void Close() {
            return warrior.Close();
        }
    }
}
