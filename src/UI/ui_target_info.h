//
// Created by desktop on 12/7/24.
//
#pragma once

#include <utility>

#include "components.h"
#include "entt/entt.hpp"
#include "mouse_control.h"
#include "quad_tree.h"
#include "utilities.h"
#include "UI/Tooltips/tooltip.h"
#include "array"
#include "Text/text.h"

namespace Target_Info {

    void Render_Info(entt::registry &zone, entt::entity &item, int &state, SDL_FPoint &mousePoint, Component::Camera &camera) {
        if (!zone.all_of<Component::Alive>(item))
            return;
        if (!zone.get<Component::Alive>(item).bIsAlive)
            return;

        constexpr int numLines = 12;
        constexpr float maxLineWidth = 500.0f;

        auto &name = zone.get<Component::Name>(item).first;

        std::string attack = "5";
        std::string armor = "15";
        std::string intelligence = "standard";
        std::string level = "lvl: 2";

        std::string damage = "2-4";
        std::string damageType = "fire";
        std::string speed = "fast";

        auto &health = zone.get<Component::Health>(item);
        std::string mana = "50/50";
        std::string XP = "23";

        std::array<std::string, numLines> stats  = {
            name,
            "ATT: " + attack + "      DEF: " + armor + "      INT: " + intelligence + "      " + level,
            "DAM: " + damage + "      " + damageType + "      SPD: " + speed,
            "HP: " + std::to_string(health.currentHealth) + "/" + std::to_string(health.maxHealth) + "      MP: " + mana + "      XP: " + XP,
            "Resistances: fire 12%%,      cold 2%%,      poison 45%%",
            "2%% chance to execute, instantly killing on every attack",
            "7%% chance to poison on hit",
            "cannot open doors",
            "insectoid",
            "melee",
            "Your odds      75%%      2-10",
            "Its odds:      50%%   for   3-7",
        };

        float lineHeight = FC_GetHeight(Graphics::fcFont, "%s", "A");
        std::array<float, numLines> spacing = {
                lineHeight * 0.35f,
                lineHeight * 0.15f,
                lineHeight * 0.15f,
                lineHeight * 0.15f,
                lineHeight * 0.15f,
                lineHeight * 0.15f,
                lineHeight * 0.15f,
                lineHeight * 0.15f,
                lineHeight * 0.15f,
                lineHeight * 0.15f,
                lineHeight * 0.15f,
                0.0f,
        };

        Tooltips::Properties<numLines> tooltipProperties = {
                stats,
                spacing,
                maxLineWidth,
                0.0f,
                0.0f,
                Tooltips::SCREEN_BOTTOM_RIGHT
        };

        Tooltips::Create_Tooltip(tooltipProperties);
    }

    void Show_Target_Info(entt::registry &zone, int &state, SDL_FPoint &mousePoint, Component::Camera &camera) {
        SDL_FRect mouseRect = Utilities::Get_FRect_From_Point_Radius(Mouse::cursorRadius, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
        Quad_Tree::Entity_Data targetData = Quad_Tree::Entity_vs_Mouse_Collision(zone, mouseRect, state);

        if (!targetData.b)
            return;

        Render_Info(zone, targetData.entity_ID, state, mousePoint, camera);
    }

}// namespace Target_Info
