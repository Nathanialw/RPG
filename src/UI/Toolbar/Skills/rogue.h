//
// Created by nathanial on 12/17/24.
//
#pragma once
#include "skills.h"

//passive skills
namespace Skill {
    namespace Rogue {

        constexpr int SIZE = 17;
        typedef bool (*Action)();

        std::array<std::pair<int , int>, SIZE> Get_Skills(entt::registry &zone) {
            std::array<std::pair<int , int>, SIZE> currentSkills = {};

            auto view = zone.view<Component::Input, Skill_Component::Skills_Points,
                    Skill_Component::Acid_Resistance,
                    Skill_Component::Archery,
                    Skill_Component::Assassination,
                    Skill_Component::Disarming,
                    Skill_Component::Dodge,
                    Skill_Component::Entomology,
                    Skill_Component::Gambling,
                    Skill_Component::Geography,
                    Skill_Component::Hunting,
                    Skill_Component::Lock_Picking,
                    Skill_Component::Pocket_Picking,
                    Skill_Component::Poison_Resistance,
                    Skill_Component::Scavenging,
                    Skill_Component::Secret_Detection,
                    Skill_Component::Stealth,
                    Skill_Component::Trap_Setting,
                    Skill_Component::Tumbling>();

            for (auto entity: view) {
                auto &Acid_Resistance = view.get<Skill_Component::Acid_Resistance>(entity);
                auto &Archery = view.get<Skill_Component::Archery>(entity);
                auto &Assassination = view.get<Skill_Component::Assassination>(entity);
                auto &Disarming = view.get<Skill_Component::Disarming>(entity);
                auto &Dodge = view.get<Skill_Component::Dodge>(entity);
                auto &Entomology = view.get<Skill_Component::Entomology>(entity);
                auto &Gambling = view.get<Skill_Component::Gambling>(entity);
                auto &Geography = view.get<Skill_Component::Geography>(entity);
                auto &Hunting = view.get<Skill_Component::Hunting>(entity);
                auto &Lock_Picking = view.get<Skill_Component::Lock_Picking>(entity);
                auto &Pocket_Picking = view.get<Skill_Component::Pocket_Picking>(entity);
                auto &Poison_Resistance = view.get<Skill_Component::Poison_Resistance>(entity);
                auto &Scavenging = view.get<Skill_Component::Scavenging>(entity);
                auto &Secret_Detection = view.get<Skill_Component::Secret_Detection>(entity);
                auto &Stealth = view.get<Skill_Component::Stealth>(entity);
                auto &Trap_Setting = view.get<Skill_Component::Trap_Setting>(entity);
                auto &Tumbling = view.get<Skill_Component::Tumbling>(entity);
                currentSkills[0] = {Acid_Resistance.level, Acid_Resistance.maxLevel};
                currentSkills[1] = {Archery.level, Archery.maxLevel};
                currentSkills[2] = {Assassination.level, Assassination.maxLevel};
                currentSkills[3] = {Disarming.level, Disarming.maxLevel};
                currentSkills[4] = {Dodge.level, Dodge.maxLevel};
                currentSkills[5] = {Entomology.level, Entomology.maxLevel};
                currentSkills[6] = {Gambling.level, Gambling.maxLevel};
                currentSkills[7] = {Geography.level, Geography.maxLevel};
                currentSkills[8] = {Hunting.level, Hunting.maxLevel};
                currentSkills[9] = {Lock_Picking.level, Lock_Picking.maxLevel};
                currentSkills[10] = {Pocket_Picking.level, Pocket_Picking.maxLevel};
                currentSkills[11] = {Poison_Resistance.level, Poison_Resistance.maxLevel};
                currentSkills[12] = {Scavenging.level, Scavenging.maxLevel};
                currentSkills[13] = {Secret_Detection.level, Secret_Detection.maxLevel};
                currentSkills[14] = {Stealth.level, Stealth.maxLevel};
                currentSkills[15] = {Trap_Setting.level, Trap_Setting.maxLevel};
                currentSkills[16] = {Tumbling.level, Tumbling.maxLevel};
            }
            return currentSkills;
        }

        Skill_Values Get_Skill_Points(entt::registry &zone) {
            auto view = zone.view<Component::Input, Skill_Component::Skills_Points>();
            for (auto entity: view) {
                auto skillPoints = view.get<Skill_Component::Skills_Points>(entity);
                return {skillPoints.unspent, skillPoints.rogueCost, skillPoints.rogueMax};
            }
        }

        std::array<std::string, SIZE> names = {
                "Acid Resistance",
                "Archery",
                "Assassination",
                "Disarming",
                "Dodge",
                "Entomology",
                "Gambling",
                "Geography",
                "Hunting",
                "Lock Picking",
                "Pocket Picking",
                "Poison Resistance",
                "Scavenging",
                "Secret Detection",
                "Stealth",
                "Trap Setting",
                "Tumbling",
        };

        std::array<std::string, SIZE> icons = {
                "acidresistanceicon",
                "archeryicon",
                "assassinationicon",
                "disarmingicon",
                "dodgeicon",
                "entomologyicon",
                "gamblingicon",
                "geographyicon",
                "huntingicon",
                "lockpickingicon",
                "pocketpickingicon",
                "poisonresistanceicon",
                "scavengingicon",
                "pitdetectionicon",
                "stealthicon",
                "trapsettingicon",
                "tumblingicon",
        };

        Skill_Tree<SIZE, Action> rogue = Skill_Tree<SIZE, Action>(names, icons, "Rogue");


        void Update(entt::registry  &zone, f2 scale) {
            rogue.Update(scale, Get_Skills(zone), Get_Skill_Points(zone));
        }

        bool Mouse_Inside() {
            rogue.Mouse_Inside();
        }

        bool Hover_Highlight() {
            return rogue.Mouse_Inside();
        }

        bool Click(entt::registry &zone, entt::entity &entity) {
            //in Event Manager check for click on skill up button
            //get index of skill
            int i = rogue.Click();
            if (i == -1)
                return false;

            auto &skills = zone.get<Skill_Component::Skills_Points>(entity);
            bool clicked = false;

            if (skills.unspent < skills.rogueCost)
                return false;

            switch (i) {
                case 0:     clicked = Increase_Skill(zone.get<Skill_Component::Acid_Resistance>(entity));   break;
                case 1:     clicked = Increase_Skill(zone.get<Skill_Component::Archery>(entity));    break;
                case 2:     clicked = Increase_Skill(zone.get<Skill_Component::Assassination>(entity));  break;
                case 3:     clicked = Increase_Skill(zone.get<Skill_Component::Disarming>(entity));   break;
                case 4:     clicked = Increase_Skill(zone.get<Skill_Component::Dodge>(entity));  break;
                case 5:     clicked = Increase_Skill(zone.get<Skill_Component::Entomology>(entity));    break;
                case 6:     clicked = Increase_Skill(zone.get<Skill_Component::Gambling>(entity));   break;
                case 7:     clicked = Increase_Skill(zone.get<Skill_Component::Geography>(entity)); break;
                case 8:     clicked = Increase_Skill(zone.get<Skill_Component::Hunting>(entity));    break;
                case 9:     clicked = Increase_Skill(zone.get<Skill_Component::Lock_Picking>(entity)); break;
                case 10:    clicked = Increase_Skill(zone.get<Skill_Component::Pocket_Picking>(entity)); break;
                case 11:    clicked = Increase_Skill(zone.get<Skill_Component::Poison_Resistance>(entity));    break;
                case 12:    clicked = Increase_Skill(zone.get<Skill_Component::Scavenging>(entity));  break;
                case 13:    clicked = Increase_Skill(zone.get<Skill_Component::Secret_Detection>(entity)); break;
                case 14:    clicked = Increase_Skill(zone.get<Skill_Component::Stealth>(entity));    break;
                case 15:    clicked = Increase_Skill(zone.get<Skill_Component::Trap_Setting>(entity));  break;
                case 16:    clicked = Increase_Skill(zone.get<Skill_Component::Tumbling>(entity));  break;
            }

            if (clicked)
                skills.unspent -= skills.rogueCost;

            return clicked;
        };

        void Render() {
            rogue.Draw();
        }

        bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
            return rogue.Toggle(toggleType);
        }

        void Close() {
            rogue.Close();
        }


    }
}