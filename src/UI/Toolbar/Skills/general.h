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

        Skill_Tree<SIZE, Action> general = Skill_Tree<SIZE, Action>(names, icons, "General");


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
            }

            if (clicked)
                skills.unspent -= skills.generalCost;

            return clicked;
        };

        void Render() {
            general.Draw();
        }

        bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
            return general.Toggle(toggleType);
        }

        void Close() {
            general.Close();
        }
    }
}