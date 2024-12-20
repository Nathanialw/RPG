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

        std::array<std::array<std::string, 5>, SIZE> descriptions = {{
                {" disallows identification of all Potions, except Health Potions.", " also allows identification of simple Potions like Curative, Restoration and others.", " allows identification of all Potions.", " further identifies if any Potion is befouled.", ""},
                {" has no effect.", " gives you +30 to hit vs Diabolics.", " also grants you 20%% Spell Point regeneration. each time you kill a Diabolic.", " also halves damage you take from Diabolics.", ""},
                {" has no effect.", " lowers damage received from Heat sources by 25%%.", " lowers it by 50%%, instead.", "lowers it by 75%%, instead.", "As a byproduct. Heat Resistance also lowers the likelihood of your catching on fire, and the duration for which you will burn if you do catch on fire, by a corresponding percentage.  If an effect can set you on fire without first causing Heat damage, your odds of avoiding the effect will correspond to the above percentage and the duration will be reduced by the same %%."},
                {" has no effect.", " grants a 25%% chance to avoid the negative effects of Archways, Fountains, Mirrors, Mosaics, Pools, Tapestries, Thrones and Stone Circles.", " grants 50%% chance, instead.", " grants 75%% chance, instead.  Also increases your odds of success when interacting with Coat of Arms squares.", ""},
                {" has no effect.", " gives a 25%% chance to resist the effects of enemy Spells and Cursed Scrolls and lowers incoming Magic damage by 25%%.", " gives a 50%% chance, and lowers incoming Magic damage by 50%%, instead.", " gives a 75%% chance, and lowers incoming Magic damage by 75%%, instead.", "Note that Magic damage is a specific damage type - not all Spells cause Magic damage."},
                {" allows a maximum of 7 Spells in your Spell Book.", " allows a maximum of 14 Spells.", " allows a maximum of 22 Spells.", " allows a maximum of 30 Spells.", "Cantrips don't count toward your Known Spells count, so the number of those you may learn is not limited."},
                {" has no effect.", " gives you a +30 to hit vs. Aberrations.", " also grants you a 20%% chance to discover a new Spell, each time you kill an Aberration.", " also halves damage you take from Aberrations in combat.", ""},
                {" has no effect.", " gives you a +30 to hit vs. Undead.", " also grants you a 20%% Health regeneration, each time you kill an Undead.", " also halves damage you take from Undead in combat.", ""},
                {" has no effect.", " grants you a 25%% chance of penning a new Spell each time you enter a new dungeon map.", " grants you a 50%% chance, instead..", " grants you a 75%% chance, instead.", ""},
                {" disallows the identification of all Scrolls.", " allows the identification of Low Power Scrolls, including Cursed Scrolls, and a 25%% chance you retain any Scroll after usage.", " allows the identification of Mid Power Scrolls, including Cursed Scrolls, and a 50%% chance you retain any Scroll after usage.", " allows the identification of High Power Scrolls, including Cursed Scrolls, and a 75%% chance you retain any Scroll after usage.", ""},
                {" disallows the casting of Spells except Cantrips.", " allows you to cast Low Power Spells.", " allows you to cast Mid Power Spells.", " allows you to cast High Power Spells.", ""},
                {" gives you zero Spell Points.", " gives you base Spell Points equal to your Intelligence, though this will be further modified by the Dungeon Level.", " increases your Spell Points by 25%%.", ", instead, increases them by 50%%.", ""},
                {" leaves a 25%% chance of spell success each time you activate a Wand, Staff or Rod.", " leaves a 50%% chance, instead, and restores 1 charge to each Wand/Staff/Rod you're carrying with each new map you enter.", " leaves a 75%% chance and restores 1 to 2 charges, instead,", " leaves a 100%% chance and restores 1 to 3 charges, instead,", ""}}
        };

        Skill_Tree<SIZE, Action> mage = Skill_Tree<SIZE, Action>(names,  icons, descriptions, "Mage");


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
                default:    break;
            }

            if (clicked)
                skills.unspent -= skills.mageCost;

            return clicked;
        };


        void Render(f2 scale) {
            mage.Draw(scale);
        }

        bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
            return mage.Toggle(toggleType);
        }

        void Close() {
            mage.Close();
        }
    }
}