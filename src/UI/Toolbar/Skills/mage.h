//
// Created by nathanial on 12/17/24.
//
#pragma once
#include "skills.h"

//passive skills
namespace  Skill {
    namespace Mage {
        constexpr int SIZE = 13;
        typedef bool (*Action)();

        std::array<std::pair<int , int>, SIZE> Get_Skills(entt::registry &zone) {
            std::array<std::pair<int , int>, SIZE> currentSkills = {};

            auto view = zone.view<Component::Input, Skill_Component::Skills_Points,
                    Skill_Component::Alchemy,
                    Skill_Component::Demonology,
                    Skill_Component::Heat_Resistance,
                    Skill_Component::History,
                    Skill_Component::Magic_Resistance,
                    Skill_Component::Memory,
                    Skill_Component::Mutology,
                    Skill_Component::Necrology,
                    Skill_Component::Research,
                    Skill_Component::Runic_Lore,
                    Skill_Component::Spell_Use,
                    Skill_Component::Thaumaturgy,
                    Skill_Component::Wand_Lore>();

            for (auto entity: view) {
                auto &Alchemy = view.get<Skill_Component::Alchemy>(entity);
                auto &Demonology = view.get<Skill_Component::Demonology>(entity);
                auto &Heat_Resistance = view.get<Skill_Component::Heat_Resistance>(entity);
                auto &History = view.get<Skill_Component::History>(entity);
                auto &Magic_Resistance = view.get<Skill_Component::Magic_Resistance>(entity);
                auto &Memory = view.get<Skill_Component::Memory>(entity);
                auto &Mutology = view.get<Skill_Component::Mutology>(entity);
                auto &Necrology = view.get<Skill_Component::Necrology>(entity);
                auto &Research = view.get<Skill_Component::Research>(entity);
                auto &Runic_Lore = view.get<Skill_Component::Runic_Lore>(entity);
                auto &Spell_Use = view.get<Skill_Component::Spell_Use>(entity);
                auto &Thaumaturgy = view.get<Skill_Component::Thaumaturgy>(entity);
                auto &Wand_Lore = view.get<Skill_Component::Wand_Lore>(entity);
                currentSkills[0] = {Alchemy.level, Alchemy.maxLevel};
                currentSkills[1] = {Demonology.level, Demonology.maxLevel};
                currentSkills[2] = {Heat_Resistance.level, Heat_Resistance.maxLevel};
                currentSkills[3] = {History.level, History.maxLevel};
                currentSkills[4] = {Magic_Resistance.level, Magic_Resistance.maxLevel};
                currentSkills[5] = {Memory.level, Memory.maxLevel};
                currentSkills[6] = {Mutology.level, Mutology.maxLevel};
                currentSkills[7] = {Necrology.level, Necrology.maxLevel};
                currentSkills[8] = {Research.level, Research.maxLevel};
                currentSkills[9] = {Runic_Lore.level, Runic_Lore.maxLevel};
                currentSkills[10] = {Spell_Use.level, Spell_Use.maxLevel};
                currentSkills[11] = {Thaumaturgy.level, Thaumaturgy.maxLevel};
                currentSkills[12] = {Wand_Lore.level, Wand_Lore.maxLevel};
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
                "Alchemy",
                "Demonology",
                "Heat Resistance",
                "History",
                "Magic Resistance",
                "Memory",
                "Mutology",
                "Necrology",
                "Research",
                "Runic Lore",
                "Spell Use",
                "Thaumaturgy",
                "Wand Lore",
        };

        std::array<std::string, SIZE> icons = {
                "alchemyicon",
                "demonologyicon",
                "fireresistanceicon",
                "historyicon",
                "magicresistanceicon",
                "memoryicon",
                "mutologyicon",
                "necromancyicon",
                "researchicon",
                "runicloreicon",
                "spelluseicon",
                "thaumaturgyicon",
                "wandloreicon",
        };

        Skill_Tree<SIZE, Action> mage = Skill_Tree<SIZE, Action>(names,  icons, "Mage");


        void Update(entt::registry  &zone, f2 scale) {
            mage.Update(scale, Get_Skills(zone), Get_Skill_Points(zone));
        }

        bool Mouse_Inside() {
            return mage.Mouse_Inside();
        }

        void Hover_Highlight() {
            mage.Mouse_Over();
        }

        bool Click(entt::registry &zone, entt::entity &entity) {
            //in Event Manager check for click on skill up button
            //get index of skill
            int i = mage.Click();
            if (i == -1)
                return false;

            auto &skills = zone.get<Skill_Component::Skills_Points>(entity);
            bool clicked = false;

            if (skills.unspent < skills.mageCost)
                return false;

            switch (i) {
                case 0:     clicked = Increase_Skill(zone.get<Skill_Component::Alchemy>(entity));   break;
                case 1:     clicked = Increase_Skill(zone.get<Skill_Component::Demonology>(entity));    break;
                case 2:     clicked = Increase_Skill(zone.get<Skill_Component::Heat_Resistance>(entity));  break;
                case 3:     clicked = Increase_Skill(zone.get<Skill_Component::History>(entity));   break;
                case 4:     clicked = Increase_Skill(zone.get<Skill_Component::Magic_Resistance>(entity));  break;
                case 5:     clicked = Increase_Skill(zone.get<Skill_Component::Memory>(entity));    break;
                case 6:     clicked = Increase_Skill(zone.get<Skill_Component::Mutology>(entity));   break;
                case 7:     clicked = Increase_Skill(zone.get<Skill_Component::Necrology>(entity)); break;
                case 8:     clicked = Increase_Skill(zone.get<Skill_Component::Research>(entity));    break;
                case 9:     clicked = Increase_Skill(zone.get<Skill_Component::Runic_Lore>(entity)); break;
                case 10:    clicked = Increase_Skill(zone.get<Skill_Component::Spell_Use>(entity)); break;
                case 11:    clicked = Increase_Skill(zone.get<Skill_Component::Thaumaturgy>(entity));    break;
                case 12:    clicked = Increase_Skill(zone.get<Skill_Component::Wand_Lore>(entity));  break;
            }

            if (clicked)
                skills.unspent -= skills.mageCost;

            return clicked;
        };


        void Render() {
            mage.Draw();
        }

        bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
            return mage.Toggle(toggleType);
        }

        void Close() {
            mage.Close();
        }
    }
}