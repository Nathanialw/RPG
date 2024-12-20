//
// Created by nathanial on 12/17/24.
//
#pragma once
#include "skills.h"


//passive skills
namespace  Skill {
    namespace General {
        constexpr int SIZE = 15;
        typedef bool (*Action)();

        std::array<std::pair<int , int>, SIZE> Get_Skills(entt::registry &zone) {
            std::array<std::pair<int , int>, SIZE> currentSkills = {};

            auto view = zone.view<Component::Input, Skill_Component::Skills_Points,
                    Skill_Component::Bartering,
                    Skill_Component::Cartography,
                    Skill_Component::Cold_Resistance,
                    Skill_Component::Cooking,
                    Skill_Component::Death_Cheating,
                    Skill_Component::Electrical_Resistance,
                    Skill_Component::Fitness,
                    Skill_Component::Gemcutting,
                    Skill_Component::Magic_Item_Use,
                    Skill_Component::Mining,
                    Skill_Component::Piety,
                    Skill_Component::Swimming,
                    Skill_Component::Self_Awareness,
                    Skill_Component::Traveling,
                    Skill_Component::Willpower>();

            for (auto entity: view) {
                auto &Bartering = view.get<Skill_Component::Bartering>(entity);
                auto &Cartography = view.get<Skill_Component::Cartography>(entity);
                auto &Cold_Resistance = view.get<Skill_Component::Cold_Resistance>(entity);
                auto &Cooking = view.get<Skill_Component::Cooking>(entity);
                auto &Death_Cheating = view.get<Skill_Component::Death_Cheating>(entity);
                auto &Electrical_Resistance = view.get<Skill_Component::Electrical_Resistance>(entity);
                auto &Fitness = view.get<Skill_Component::Fitness>(entity);
                auto &Gemcutting = view.get<Skill_Component::Gemcutting>(entity);
                auto &Magic_Item_Use = view.get<Skill_Component::Magic_Item_Use>(entity);
                auto &Mining = view.get<Skill_Component::Mining>(entity);
                auto &Piety = view.get<Skill_Component::Piety>(entity);
                auto &Self_Awareness = view.get<Skill_Component::Self_Awareness>(entity);
                auto &Swimming = view.get<Skill_Component::Swimming>(entity);
                auto &Traveling = view.get<Skill_Component::Traveling>(entity);
                auto &Willpower = view.get<Skill_Component::Willpower>(entity);
                currentSkills[0] = {Bartering.level, Bartering.maxLevel};
                currentSkills[1] = {Cartography.level, Cartography.maxLevel};
                currentSkills[2] = {Cold_Resistance.level, Cold_Resistance.maxLevel};
                currentSkills[3] = {Cooking.level, Cooking.maxLevel};
                currentSkills[4] = {Death_Cheating.level, Death_Cheating.maxLevel};
                currentSkills[5] = {Electrical_Resistance.level, Electrical_Resistance.maxLevel};
                currentSkills[6] = {Fitness.level, Fitness.maxLevel};
                currentSkills[7] = {Gemcutting.level, Gemcutting.maxLevel};
                currentSkills[8] = {Magic_Item_Use.level, Magic_Item_Use.maxLevel};
                currentSkills[9] = {Mining.level, Mining.maxLevel};
                currentSkills[10] = {Piety.level, Piety.maxLevel};
                currentSkills[11] = {Self_Awareness.level, Self_Awareness.maxLevel};
                currentSkills[12] = {Swimming.level, Swimming.maxLevel};
                currentSkills[13] = {Traveling.level, Traveling.maxLevel};
                currentSkills[14] = {Willpower.level, Willpower.maxLevel};
            }
            return currentSkills;
        }

        Skill_Values Get_Skill_Points(entt::registry &zone) {
            auto view = zone.view<Component::Input, Skill_Component::Skills_Points>();
            for (auto entity: view) {
                auto skillPoints = view.get<Skill_Component::Skills_Points>(entity);
                return {skillPoints.unspent, skillPoints.generalCost, skillPoints.generalMax};
            }
        }

        std::array<std::string, SIZE> names = {
                "Bartering",
                "Cartography",
                "Cold Resistance",
                "Cooking",
                "Death Cheating",
                "Electrical Resistance",
                "Fitness",
                "Gemcutting",
                "Magic Item Use",
                "Mining",
                "Piety",
                "Self Awareness",
                "Swimming",
                "Traveling",
                "Willpower",
        };

        std::array<std::string, SIZE> icons = {
                "barteringicon",
                "cartographyicon",
                "coldresistanceicon",
                "cookingicon",
                "deathcheatingicon",
                "electricalresistanceicon",
                "fitnessicon",
                "gemcuttingicon",
                "magicitemuseicon",
                "miningicon",
                "pietyicon",
                "selfawarenessicon",
                "swimmingicon",
                "travelingicon",
                "willpowericon",
        };

        std::array<std::array<std::string, 5>, SIZE> descriptions = {
                {{" has no effect.", " means Merchants, Smugglers and other friendly NPCs offer 25%% better deals.", " means they offer 50%% better deals, instead.", " means they offer 75%% better deals, instead.", ""},
                 {" means Maps have no effect.", " means Maps show features except Pit Traps, Secrets, Booby Traps or Cave Ins.", " means Maps show features except Booby Traps or Cave Ins.", " means Maps show everything", "Cartography Skill also effects the details shown by special maps like Dwarven Maps and Treasure Maps."},
                 {" has no effect.", " lowers damage received from Cold sources by 25%%.", " lowers it by 50%%, instead.", " lowers it by 75%%, instead.", "As a byproduct, Cold Resistance also lowers the likelihood of you becoming Frozen, and the duration you will remain Frozen if you do, by a corresponding percentage.  If an effect can freeze you without first causing Cold damage, your odds of avoiding the effect will correspond to the above percentage and the duration will be reduced by the same precentage."},
                 {" allows the cooking of any Uncooked meat to 20%% of its maximum potential quality.", " allows the cooking of any Uncooked meat to 40%% of its maximum potential quality.", " allows the cooking of any Uncooked meat to 60%% of its maximum potential quality.", " allows the cooking of any Uncooked meat to 80%% of its maximum potential quality.", ""},
                 {" has no effect.", " gives you a 25%% chance to avoid insta-kill effects (like those of a Basilisk, T-Rex or Altar).", " gives you a 50%% chance, instead.", " gives you a 75%% chance, instead.", ""},
                 {" has no effect.", " lowers damage received from Electrical sources by 25%%.", " lowers it by 50%%, instead.", " lowers it by 75%%, instead.", "As a byproduct, Electrical Resistance also lowers the likelihood of you becoming Electrified, and the duration you will remain so if you do, by a corresponding percentage.  If an effect can Electrify you without first causing Electric damage, your odds of avoiding the effect will correspond to the above percentage and the duration will be reduced by the same precentage."},
                 {" adds 2 to your Max Health with each Character Level you gain.", " adds 2 to 3, instead.", " adds 2 to 4, instead.", " adds 2 to 5, instead.", ""},
                 {" has no effect.", " increases the value of Gems you find by 50%%.", " doubles the value, instead.", " triples the value, instead.", "It will not affect the value of Gems earned from sales or gambling, or from Transmuting Objects to Gems, though it will affect the value gained from Pocket Picking."},
                 {" disallows the use of Magic Items or Magic Jewellery.", " allows the use of Low Power Magic Items and Magic Jewellery.", " allows the use of Mid Power Magic Items and Magic Jewellery.", " allows the use of High Power Magic Items and Magic Jewellery.", ""},
                 {" grants a 25%% chance to mine mineable walls..", " grants a 25%% chance to avert Cave Ins and a 50%% chance to mine mineable walls without triggering a Cave In.", " grants a 50%% chance to avert Cave Ins and a 75%% chance to mine mineable walls without triggering a Cave In", " grants a 75%% chance to avert Cave Ins and a 90%% chance to mine mineable walls without triggering a Cave In", ""},
                 {" has no effect.", " gives a 33%% chance tof improving divine relationships by 1 extra percentage point, each time they improve.  It also grants a 25%% chance any Prayer isn't destroyed on use.", " gives a 66%% chance of improving divine relationships and a 50%% chance of preserving Prayers.", " gives a 100%% chance of improving divine relationships and a 75%% chance of preserving Prayers.", ""},
                 {" has no effect.", " gives you a 25%% chance to avoid effects that drain your Strength, Dexterity, Intelligence or Max Health (excluding Lycanthropy).  If it fails to prevent the effect, the amount of points drained will be lowered by 25%%.", " gives you a 50%% chance, and lowers the amount drained by 50%%, instead.", " gives you a 75%% chance, and lowers the amount drained by 75%%, instead.", ""},
                 {" gives a 25%% chance of avoiding drifting or drowning in Water, and of drifting in Acid or Lava.", " gives a 50%% chance, instead.", " gives a 75%% chance, instead.", " gives a 100%% chance, instead.", ""},
                 {" allows 10 slots in your Inventory.", " allows 15 slots, instead.", " allows 20 slots, instead.", " allows 25 slots, instead.", ""},
                 {" has no effect.", "gives you a 25%% chance to avoid effects that drain your Experience or Spell Points (like Wraiths and Archways). If it fails to prevent the effect, the amount of points drained will be lowered by 25%%.", " gives you a 50%% chance, and lowers the amount drained by 50%%, instead.", " gives you a 75%% chance, and lowers the amount drained by 75%%, instead.", ""},}
        };

        Skill_Tree<SIZE, Action> general = Skill_Tree<SIZE, Action>(names, icons, descriptions, "General");


        void Update(entt::registry  &zone, f2 scale) {
            general.Update(scale, Get_Skills(zone), Get_Skill_Points(zone));
        }

        void Hover_Highlight() {
            general.Mouse_Over();
        }

        bool  Mouse_Inside() {
            return general.Mouse_Inside();
        }

        bool Click(entt::registry &zone, entt::entity &entity) {
            //in Event Manager check for click on skill up button
            //get index of skill
            int i = general.Click();
            if (i == -1)
                return false;

            auto &skills = zone.get<Skill_Component::Skills_Points>(entity);
            bool clicked = false;

            if (skills.unspent < skills.generalCost)
                return false;

            switch (i) {
                case 0:     clicked = Increase_Skill(zone.get<Skill_Component::Bartering>(entity));   break;
                case 1:     clicked = Increase_Skill(zone.get<Skill_Component::Cartography>(entity));    break;
                case 2:     clicked = Increase_Skill(zone.get<Skill_Component::Cold_Resistance>(entity));  break;
                case 3:     clicked = Increase_Skill(zone.get<Skill_Component::Cooking>(entity));   break;
                case 4:     clicked = Increase_Skill(zone.get<Skill_Component::Death_Cheating>(entity));  break;
                case 5:     clicked = Increase_Skill(zone.get<Skill_Component::Electrical_Resistance>(entity));    break;
                case 6:     clicked = Increase_Skill(zone.get<Skill_Component::Fitness>(entity));   break;
                case 7:     clicked = Increase_Skill(zone.get<Skill_Component::Gemcutting>(entity)); break;
                case 8:     clicked = Increase_Skill(zone.get<Skill_Component::Magic_Item_Use>(entity));    break;
                case 9:     clicked = Increase_Skill(zone.get<Skill_Component::Mining>(entity)); break;
                case 10:    clicked = Increase_Skill(zone.get<Skill_Component::Piety>(entity)); break;
                case 11:    clicked = Increase_Skill(zone.get<Skill_Component::Swimming>(entity));    break;
                case 12:    clicked = Increase_Skill(zone.get<Skill_Component::Self_Awareness>(entity));  break;
                case 13:    clicked = Increase_Skill(zone.get<Skill_Component::Traveling>(entity)); break;
                case 14:    clicked = Increase_Skill(zone.get<Skill_Component::Willpower>(entity));    break;
                default:    break;
            }

            if (clicked)
                skills.unspent -= skills.generalCost;

            return clicked;
        };

        void Render(f2 scale) {
            general.Draw(scale);
        }

        bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
            return general.Toggle(toggleType);
        }

        void Close() {
            general.Close();
        }
    }
}