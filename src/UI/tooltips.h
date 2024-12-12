#pragma once

#include "Debug/debug_components.h"
#include "components.h"
#include "graphics.h"
#include "item_components.h"
#include "ui.h"

namespace Tooltip {

    struct Tooltip_Render_Data {
        std::string text;
        Graphics::Surface_Data spriteData{};
        SDL_FRect renderRect{};
        SDL_Color color{};
    };

    struct Tooltip_Text_Data {
        std::string text;
        SDL_FPoint renderPoint;
        float charWidth;
        float charHeight;
        float tooltipWidth = 0.0f;
        float currentRow = 0.0f;
    };

    Tooltip_Render_Data Create_Text(Tooltip_Text_Data &tooltip, SDL_Color &textColor, const int &rows) {
        Graphics::Surface_Data itemName = Graphics::Load_Text_Texture(tooltip.text, textColor);

        SDL_FRect statNameRect = {tooltip.renderPoint.x, tooltip.renderPoint.y};
        statNameRect.y = (tooltip.renderPoint.y - (rows * tooltip.charHeight)) + tooltip.currentRow;
        statNameRect.h = tooltip.charHeight;
        statNameRect.w = tooltip.text.size() * tooltip.charWidth;

        //adds an extra char width to because the chars are taller than they are wide
        if (tooltip.tooltipWidth < statNameRect.w)
            tooltip.tooltipWidth = statNameRect.w;

        tooltip.currentRow += tooltip.charHeight;

        return {tooltip.text, itemName, statNameRect, textColor};
    }

    void Render_Tooltip_Background(SDL_FRect &tooltipBox, Component::Camera &camera) {
        SDL_Rect sourceRect = {0, 0, 48, 48};
        SDL_FRect tooltipBackground = tooltipBox;

        float tooltipBorder = 5.0f / camera.scale.y;
        tooltipBackground.x -= tooltipBorder;
        tooltipBackground.y -= (tooltipBackground.h + tooltipBorder);
        tooltipBackground.w += (tooltipBorder * 2.0f);
        tooltipBackground.h += (tooltipBorder * 2.0f);

        Graphics::Render_FRect(Graphics::tooltipBackground, Color::white, &sourceRect, &tooltipBackground);
    }

    void Render_Tooltip_FC(Component::Camera &camera, SDL_FRect &statBox, float &charHeight, SDL_Color color, std::string &text) {
        FC_Scale scale = {1.0f / camera.scale.x, 1.0f / camera.scale.y};
        SDL_FRect statNameRect = statBox;
        FC_DrawScale_Center(Graphics::fcFont, Graphics::renderer, statNameRect.x, statNameRect.y, scale, color, text.c_str());
        statBox.y += charHeight;
    }

    void Render_Tooltip(entt::registry &zone, entt::entity &item, int &state, SDL_FPoint &mousePoint, Component::Camera &camera) {
        auto &name = zone.get<Item_Component::Name>(item).name;
        auto &stats = zone.get<Item_Component::Item_Stats>(item).stats;
        auto &rarity = zone.get<Item_Component::Rarity>(item);
        SDL_Color rarityColor = Item_Component::rarityColor[rarity];
        int rows = 2 + (int) stats.size();

        std::vector<Tooltip_Render_Data> renderArray(rows);
        int renderArrayIndex = 0;

        //render item name at the top
        Tooltip_Text_Data tooltip = {name, mousePoint};
        tooltip.charWidth = (10.0f / camera.scale.x);
        tooltip.charHeight = (20.0f / camera.scale.y);

        //set name at the color of it's rarity
        renderArray[renderArrayIndex] = Create_Text(tooltip, rarityColor, rows);
        renderArrayIndex++;

        //set type at the color of it's rarity
        tooltip.text = Item_Component::Get_Item_Type_String(zone.get<Item_Component::Item_Type>(item));
        renderArray[renderArrayIndex] = Create_Text(tooltip, rarityColor, rows);
        renderArrayIndex++;

        for (auto &stat: stats) {
            //render each stat in order
            std::string statName = Item_Component::statName[stat.first];
            std::string statValue = std::to_string(stat.second);
            std::string statData = statName + "    +" + statValue;
            tooltip.text = statData;

            renderArray[renderArrayIndex] = Create_Text(tooltip, Color::blueMagical, rows);
            renderArrayIndex++;
        }
        //render tooltip background
        SDL_FRect tooltipBackground;
        //    if (Debug::settings[Debug::Settings::fontRenderFC]) {
        //      tooltipBackground = {mousePoint.x, mousePoint.y, tooltip.tooltipWidth, (tooltip.charHeight * (stats.size() + 2))};
        //    } else {
        float x = mousePoint.x - (tooltip.tooltipWidth / 2.0f);
        tooltipBackground = {x, mousePoint.y, tooltip.tooltipWidth, (tooltip.charHeight * (stats.size() + 2))};
        //    }

        Render_Tooltip_Background(tooltipBackground, camera);
        //render item stats
        float charHeight = tooltip.charHeight;
        for (auto row: renderArray) {
            //      Debug::settings[Debug::Settings::fontRenderFC] ?
            //                                                     Render_tooltip_0(row, tooltip.tooltipWidth) :
            Render_Tooltip_FC(camera, row.renderRect, charHeight, row.color, row.text);
            SDL_DestroyTexture(row.spriteData.pTexture);
        }
    }

    void Show_Item_Tooltip(entt::registry &zone, entt::entity &entity, int &state, SDL_FPoint &mousePoint, Component::Camera &camera) {
        if (Bag_UI::bToggleCharacterUI) {
            if (Mouse::bRect_inside_Cursor(Bag_UI::Character_UI)) {
                entt::entity item = Item_Component::emptyEquipSlot[state];
                if (Bag_UI::Is_Cursor_Inside_Bag_Area(zone, camera, mousePoint)) {//if mouse is over the bag area
                    int mouseoverSlot = Bag_UI::Get_Bag_Slot(zone, mousePoint, camera);
                    auto &bag = zone.get<Component::Bag>(entity).bag;
                    item = bag[mouseoverSlot];
                } else if (Equipment_UI::Mouse_Inside_Equipment_Screen(zone, camera, mousePoint)) {// if mouse is over the equip area
                    item = Equipment_UI::Get_Equip_Slot(zone, state, camera);
                }
                if (item != Bag_UI::emptyBagSlot[state] && item != Item_Component::emptyEquipSlot[state]) {
                    Tooltip::Render_Tooltip(zone, item, state, mousePoint, camera);
                }
            }
        }
    }
}// namespace Tooltip
