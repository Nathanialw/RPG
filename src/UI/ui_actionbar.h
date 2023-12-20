#pragma once
#include "entt/entt.hpp"
#include "graphics.h"
#include "item_components.h"
#include "ui_elements.h"

namespace Action_Bar {
  struct On_Spellbar {
  };

  struct Spell_Bar {
    std::array<entt::entity, 10> spell;
    SDL_FRect actionBarFrame = {0.0f, 0, 640.0f, 64.0f};
  };

  Spell_Bar actionBar;

  void Update_Position(SDL_FRect &frameRect) {
    SDL_Rect rect = Utilities::SDL_FRect_To_SDL_Rect(frameRect);
    frameRect = UI::Center_Rect(rect);
    int w;
    int h;
    SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
    frameRect.y = (float) h - frameRect.h;
  }

  void Create_Action_Bar(entt::registry &zone) {
    // initialize value of empty
    entt::entity emptyBarSlot = zone.create();
    zone.emplace<On_Spellbar>(emptyBarSlot);
    auto &icon = zone.emplace<Component::Icon>(emptyBarSlot, Graphics::emptyBagIcon, Graphics::default_icon, Item_Component::rarityBorder[Item_Component::Rarity::common], Graphics::bagSlotBorder);
//    auto &icon = zone.emplace<Component::Icon>(emptyBarSlot, Graphics::emptyBagIcon, nullptr, Item_Component::rarityBorder[Item_Component::Rarity::common], Graphics::bagSlotBorder);
    icon.clipSprite = {0, 0, 256, 256};
    icon.clipIcon = {0, 0, 256, 256};
    icon.renderRectSize = {64.0f, 64.0f};
    icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};

    //    initialize action bar spell slots
    for (int i = 0; i < actionBar.spell.size(); i++) {
      actionBar.spell[i] = emptyBarSlot;
    }
    Update_Position(actionBar.actionBarFrame);
  }

   bool Get_Slot(entt::registry &zone, Component::Camera &camera) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBarFrame);
    for (int i = 0; i < actionBar.spell.size(); i++) {
      auto &icon = zone.get<Component::Icon>(actionBar.spell[i]);
      SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};
      if (Mouse::bRect_inside_Cursor(renderRect)) {
        Utilities::Log((int)actionBar.spell[i]);
        return true;
      }
    }
    return false;
  }

  void Render_Action_Bar(entt::registry &zone, Component::Camera &camera) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBarFrame);

    for (int i = 0; i < actionBar.spell.size(); i++) {
      auto &icon = zone.get<Component::Icon>(actionBar.spell[i]);
      SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};
      SDL_RenderCopyF(Graphics::renderer, icon.pTexture, &icon.clipIcon, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pIconRarityBorder, &icon.clipIcon, &renderRect);
    }
  }
}// namespace Action_Bar