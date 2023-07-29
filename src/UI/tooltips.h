#pragma once

#include "item_components.h"
#include "components.h"
#include "graphics.h"

namespace Tooltip {

  struct Tooltip_Render_Data {
    Graphics::Surface_Data spriteData;
    SDL_FRect renderRect;
  };

  struct Tooltip_Text_Data {
    std::string text;
    SDL_FPoint renderPoint;
    float charWidth;
    float charHeight;
    float tooltipWidth = 0.0f;
    float currentRow = 0.0f;
  };

  Tooltip_Render_Data Create_Text(Tooltip_Text_Data &tooltip, SDL_Color &textcolor, int &rows) {
    Graphics::Surface_Data itemName = Graphics::Load_Text_Texture(tooltip.text, textcolor);

    SDL_FRect statNameRect = {tooltip.renderPoint.x, tooltip.renderPoint.y};
    statNameRect.y = (tooltip.renderPoint.y - (rows * tooltip.charHeight)) + tooltip.currentRow;
    statNameRect.h = tooltip.charHeight;
    statNameRect.w = tooltip.text.size() * tooltip.charWidth;

    //adds an extra char width to because the chars are  taller than they are wide
    if (tooltip.tooltipWidth < ((tooltip.text.size() + 1.0f) * tooltip.charWidth)) {
      tooltip.tooltipWidth = ((tooltip.text.size() + 1.0f) * tooltip.charWidth);
    }
    tooltip.currentRow += tooltip.charHeight;

    return {itemName, statNameRect};
  }

  void Render_Tooltip_Background(SDL_FRect &tooltipBox, Component::Camera &camera) {
    SDL_Rect sourcerect = {0, 0, 48, 48};
    SDL_FRect tooltipBackground = tooltipBox;

    float tooltipBorder = 5.0f / camera.scale.y;
    tooltipBackground.x -= tooltipBorder;
    tooltipBackground.y -= (tooltipBackground.h + tooltipBorder);
    tooltipBackground.w += (tooltipBorder * 2.0f);
    tooltipBackground.h += (tooltipBorder * 2.0f);

    Graphics::Render_FRect(Graphics::tooltipBackground, {255, 255, 255}, &sourcerect, &tooltipBackground);
  }

  void Show_Item_Tooltip(entt::registry &zone, SDL_FPoint &mousePoint, Component::Camera &camera) {

    if (UI::bToggleCharacterUI == true) {
      if (Mouse::bRect_inside_Cursor(UI::Character_UI)) {

        entt::entity item = Item_Component::emptyEquipSlot;
        if (UI::Bag_UI::Is_Cursor_Inside_Bag_Area(zone, camera, mousePoint) == true) { //if mouse is over the bag area
          int mouseoverSlot = UI::Bag_UI::Get_Bag_Slot(zone, mousePoint, camera);
          item = UI::Bag_UI::UI_bagSlots[mouseoverSlot];
        } else if (UI::Equipment_UI::Mouse_Inside_Equipment_Screen(zone, camera, mousePoint)) { // if mouse is over the equip area
          item = UI::Equipment_UI::Get_Equip_Slot(zone, camera);
        }
        if (item != UI::Bag_UI::emptyBagSlot && item != Item_Component::emptyEquipSlot) {

          auto &name = zone.get<Item_Component::Name>(item).name;
          auto &stats = zone.get<Item_Component::Item_Stats>(item).stats;
          auto &rarity = zone.get<Item_Component::Rarity>(item);
          SDL_Color blue = {51, 153, 255, 255};
          SDL_Color rarityColor = Item_Component::rarityColor[rarity];
          int rows = 1 + (int) stats.size();


          std::vector<Tooltip_Render_Data> renderArray(rows);
          int renderArrayIndex = 0;

          //render item name at the top
          Tooltip_Text_Data tooltip = {name, mousePoint};
          tooltip.charWidth = (10.0f / camera.scale.x);
          tooltip.charHeight = (20.0f / camera.scale.y);

          //set name at the color of it's rarity
          renderArray[renderArrayIndex] = Create_Text(tooltip, rarityColor, rows);
          renderArrayIndex++;

          for (auto &stat: stats) {
            //render each stat in order
            std::string statName = Item_Component::statName[stat.first];
            std::string statValue = std::to_string(stat.second);
            std::string statData = statName + "    +" + statValue;

            tooltip.text = statData;

            renderArray[renderArrayIndex] = Create_Text(tooltip, blue, rows);
            renderArrayIndex++;
          }
          //render tooltip background
          SDL_FRect tooltipBackground = {mousePoint.x, mousePoint.y, tooltip.tooltipWidth, (tooltip.charHeight * (stats.size() + 1))};
          Render_Tooltip_Background(tooltipBackground, camera);
          //render item stats
          for (auto &row: renderArray) {
            row.renderRect.x += (tooltip.tooltipWidth - row.renderRect.w) / 2.0f;
            Graphics::Render_FRect(row.spriteData.pTexture, {255, 255, 255}, &row.spriteData.k, &row.renderRect);
            SDL_DestroyTexture(row.spriteData.pTexture);
          }
        }
      }
    }
  }
}
