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
#include "tooltips.h"
#include "array"

namespace Target_Info {

    const int rows = 13;


    void AddText(std::array<Tooltip::Tooltip_Render_Data, rows> &renderArray, int &renderArrayIndex, Tooltip::Tooltip_Text_Data &tooltip, SDL_Color color, std::string line = " ") {
        tooltip.text = line;
        renderArray[renderArrayIndex] = Create_Text(tooltip, color, rows);
        renderArrayIndex++;
    }

    void Render_Info(entt::registry &zone, entt::entity &item, int &state, SDL_FPoint &mousePoint, Component::Camera &camera) {
        if (!zone.all_of<Component::Alive>(item))
            return;
        if (!zone.get<Component::Alive>(item).bIsAlive)
            return;

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

        std::string resistances = "Resistances: fire 12%%, cold 2%%, poison 45%%,";

        std::string periodicEffects = "2%% chance to execute, instantly killing on every attack";

        std::string onHitEffects = "7%% chance to poison on hit";

        std::string specialProperties = "cannot open doors";

        std::string type = "insectoid";

        std::string meleeRanged = "melee";

        //mathed out values
        std::string chanceToHit = "Your odds: 75%% 2-10 ";
        std::string chanceToBeHitBy = "Its odds: 50%% for 3-7";


        SDL_Color rarityColor = Item_Component::rarityColor[Rarity::common];

        std::array<Tooltip::Tooltip_Render_Data, rows> renderArray;
        int renderArrayIndex = 0;

        //render item name at the top
        Tooltip::Tooltip_Text_Data tooltip;
        SDL_FPoint position = {1300.0f, camera.screen.h};

        tooltip.renderPoint = position;
        tooltip.charWidth = (10.0f / camera.scale.x);
        tooltip.charHeight = (20.0f / camera.scale.y);

        //set name at the color of it's rarity
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, name);

        auto line = "ATT:" + attack + " DEF" + armor + " INT" + intelligence + " " + level;
        AddText(renderArray,  renderArrayIndex, tooltip, rarityColor, line);

        line = "DAM: " + damage + " " + damageType + " SPD" + speed;
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, line);

        line = "HP: " + std::to_string(health.currentHealth) + "/" + std::to_string(health.maxHealth) + " MP: " + mana + " XP: " + XP;
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, line);

        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, resistances);
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, periodicEffects);
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, onHitEffects);
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, specialProperties);
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, type);
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, meleeRanged);
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor);
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, chanceToHit);
        AddText(renderArray, renderArrayIndex, tooltip, rarityColor, chanceToBeHitBy);

        //render tooltip background
        SDL_FRect tooltipBackground;
        float x = position.x - (tooltip.tooltipWidth / 2.0f);
        tooltipBackground = {x, position.y, tooltip.tooltipWidth, (tooltip.charHeight * renderArrayIndex)};
        Tooltip::Render_Tooltip_Background(tooltipBackground, camera.scale);

        //render item stats
        float charHeight = tooltip.charHeight;
        for (auto row: renderArray) {
            Tooltip::Render_Tooltip_FC(camera, row.renderRect, charHeight, row.color, row.text);
            SDL_DestroyTexture(row.spriteData.pTexture);
        }
    }

    void Show_Target_Info(entt::registry &zone, int &state, SDL_FPoint &mousePoint, Component::Camera &camera) {
        SDL_FRect mouseRect = Utilities::Get_FRect_From_Point_Radius(Mouse::cursorRadius, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
        Quad_Tree::Entity_Data targetData = Quad_Tree::Entity_vs_Mouse_Collision(zone, mouseRect, state);

        if (!targetData.b)
            return;

        Render_Info(zone, targetData.entity_ID, state, mousePoint, camera);
    }

}// namespace Target_Info
