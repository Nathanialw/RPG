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

        std::array<std::array<std::string, 5>, SIZE> descriptions = {
                {{" has no effect.", " gives a 25%% chance to avoid having Objects melted by Acid, and lowers incoming Acid damage by 25%%.", " raises these resistances to 50%%, instead.", " raises these resistances to 70%%, instead.", ""},
                 {" has no effect.", " adds 3 to the damage you do while using any Missile Weapon.", " also gives you +35 to hit while using any Missile Weapon", " also adds 25%% to the damage done by any Missile Weapon.", "Archery may also stand in for any Weapon Use when equipping Missile Weapons, so long as your Class can attain an equivalent Weapon Use Skill level."},
                 {" has no effect.", " gives you +30 to hit vs Persons.", " gives you a bounty of 1 to 10x Dungeon Level in Gold Pieces, with each kill of a Person.", " also halves damage you take from Persons in combat.", ""},
                 {" has no effect.", " grants a 25%% chance to ignore the effects of any Booby Trap at the moment you spring it.", " grants a 50%% chance, instead.", " grants a 75%% chance, instead.", "It has no effect on Pit Traps of any kind, as they are in a separate class from Booby Traps."},
                 {" has no effect.", " adds 15 to your Defence.", "adds 30, instead.", "adds 45, instead.", ""},
                 {" has no effect.", " gives you a +30 to hit vs. Insectoids.", " also grants you a 30%% chance to brew a random Potion, each time you kill an Insectoid.", " also halves damage you take from Insectoids in combat.", ""},
                 {" gives you a 50%% chance to win when gambling at Wheels of Fortune.", " gives you a 57%% chance, instead.", "gives you a 64%% chance, instead.", " gives you a 71%% chance, instead.", ""},
                 {" has no effect.", " grants a 15%% chance to find a map each time you enter a new Dungeon Map.", " grants a 30%% chance, instead.", " grants a 45%% chance, instead.", "At least Basic Cartography is required to read any map you find."},
                 {" has no effect.", " gives you +30 to hit vs Animals.", " also grants you a 20%% chance to have your Health restored to Max, each time you kill an Animal.", " also halves damage you take from Animals.", ""},
                 {" has no effect.", " gives a 25%% chance to unlock any locked door or container with each attempt.", " gives a 50%% chance of success, instead.", " gives a 75%% chance of success, instead.", "Failure when trying to pick a lock always leaves a 50%% chance the lock Jammed, which will disallow further attempts at Lock Picking on that door or container."},
                 {" has no effect.", " gives a 25%% chance to steal from Merchants, Smugglers and other NPCs the first time you meet.", " gives a 50%% chance, instead.", " gives a 75%% chance, instead.", "What or how much you steal will be randomly determined, but will correspond to the type of NPC you're stealing from.  (I.e. Armour from Armour Merchants, Mundane Items from Mundane Item Merchants, etc.)"},
                 {" leaves a 5%% chance a Poison leaving your system..", " gives a 25%% chance to avoid being Poisoned, lowers incoming Poison damage by 25%% and increases the odds of Poison leaving your system by 3%%.", " gives a 50%% chance to avoid being Poisoned, lowers incoming Poison damage by 50%% and increases the odds of Poison leaving your system by 6%%, instead.", " gives a 75%% chance to avoid being Poisoned, lowers incoming Poison damage by 75%% and increases the odds of Poison leaving your system by 9%%, instead.", ""},
                 {" has no effect.", "adds 10 Gold Pieces to the amount found whenever you find Gold.", " adds 20 Gold Pieces, instead.", " adds 30 Gold Pieces, instead", "It will not affect the Gold value of other found treasure, like Silver or Gems, nor of that obtained through means that directly conjure Gold magically, though it willl affect Gold obtained through Pocket Picking or collected bounties."},
                 {" grants a 10%% chance to identify nearby Concealed or Secret Doors/Compartments, Cave Ins or Buried Objects.", " grants a 25%% chance to identify nearby Pit Traps, Concealed or Secret Doors/Compartments, Cave Ins or Buried Objects.", " grants a 50%% chance, instead.", " grants a 75%% chance, instead.", ""},
                 {" allows most monsters to see you up to 12 squares distant, hear you from 1 to 12 squares distant.", " lowers those ranges by 3 squares.", " lowers those ranges by 6 squares, instead.", " lowers those ranges by 9 squares, instead.", "Stealth has no effect on Genius or Automaton monsters, both of which always know where you are, and will not reduce distance at which you can be seen if you have a light source."},
                 {" leaves a 50%% chance of successfully setting a Trap with a Trap Kit, without accidentally triggering its effects on yourself.", " leaves a 65%% chance, instead.", " leaves a 85%% chance, instead.", "leaves a 100%% chance, instead.", "Monsters killed by Friendly Traps do not adjust Monster Attitudes."},
                 {" has no effect.", " lowers damage you take from falling by 25%%.", " lowers it by 50%%, instead.", " lowers it by 75%%, instead.", ""}}
        };

        Skill_Tree<SIZE, Action> rogue = Skill_Tree<SIZE, Action>(names, icons, descriptions, "Rogue");

        void Update(entt::registry  &zone, f2 scale) {
            rogue.Update(scale, Get_Skills(zone), Get_Skill_Points(zone));
        }

        bool Mouse_Inside() {
            rogue.Mouse_Inside();
        }

        void Hover_Highlight() {
            rogue.Mouse_Over();
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
                default:    break;
            }

            if (clicked)
                skills.unspent -= skills.rogueCost;

            return clicked;
        };

        void Render(f2 scale) {
            rogue.Draw(scale);
        }

        bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
            return rogue.Toggle(toggleType);
        }

        void Close() {
            rogue.Close();
        }
    }
}