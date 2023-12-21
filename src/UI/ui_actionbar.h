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
  entt::entity defaultSlot;

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
    defaultSlot = zone.create();
    zone.emplace_or_replace<On_Spellbar>(defaultSlot);
    auto &icon = zone.emplace_or_replace<Component::Icon>(defaultSlot, Graphics::emptyBagIcon, Graphics::default_icon, Item_Component::rarityBorder[Item_Component::Rarity::common], Graphics::bagSlotBorder);
    //    auto &icon = zone.emplace_or_replace<Component::Icon>(emptyBarSlot, Graphics::emptyBagIcon, nullptr, Item_Component::rarityBorder[Item_Component::Rarity::common], Graphics::bagSlotBorder);
    icon.clipSprite = {0, 0, 256, 256};
    icon.clipIcon = {0, 0, 256, 256};
    icon.renderRectSize = {64.0f, 64.0f};
    icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};

    //    initialize action bar spell slots
    for (int i = 0; i < actionBar.spell.size(); i++) {
      actionBar.spell[i] = defaultSlot;
    }
    Update_Position(actionBar.actionBarFrame);
  }

  bool Mouse_Inside_Actionbar(Component::Camera &camera) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBarFrame);
    if (Mouse::bRect_inside_Cursor(renderBarFrame)){
      return true;
    }
    return false;
  }

  void Set_Mouse_Spell_On_Actionbar(entt::registry &zone, Component::Camera &camera) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBarFrame);

    for (int i = 0; i < actionBar.spell.size(); i++) {
      auto &icon = zone.get<Component::Icon>(actionBar.spell[i]);
      SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};
      if (Mouse::bRect_inside_Cursor(renderRect)) {
        if (zone.get<Component::On_Mouse>(Mouse::mouseItem).type == Component::Icon_Type::spell) {
          actionBar.spell[i] = Mouse::mouseItem;
          zone.remove<Component::On_Mouse>(Mouse::mouseItem);
          Mouse::itemCurrentlyHeld = false;
          Mouse::mouseItem = Mouse::cursor_ID;
          return;
        }
      }
    }
  }

  void Get_Mouse_Spell_From_Actionbar(entt::registry &zone, Component::Camera &camera) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBarFrame);

    for (int i = 0; i < actionBar.spell.size(); i++) {
      auto &icon = zone.get<Component::Icon>(actionBar.spell[i]);
      SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};
      //instead we need a default slot entity that to check against and set as if it is already set as a spell
      if (Mouse::bRect_inside_Cursor(renderRect)) {
        if (actionBar.spell[i] != defaultSlot) {
          Mouse::mouseItem = actionBar.spell[i];
          actionBar.spell[i] = defaultSlot;
          auto &mouseItem = zone.emplace_or_replace<Component::On_Mouse>(Mouse::mouseItem);
          mouseItem.type = Component::Icon_Type::spell;
          Mouse::itemCurrentlyHeld = true;
          return;
        }
      }
    }
  }

  void Clear_Spell_On_Mouse(entt::registry &zone) {
    if (Mouse::mouseItem == Mouse::cursor_ID) {
      Utilities::Log("same");
    }
    if (zone.get<Component::On_Mouse>(Mouse::mouseItem).type == Component::Icon_Type::spell) {
      zone.remove<Component::On_Mouse>(Mouse::mouseItem);
      Mouse::itemCurrentlyHeld = false;
      Mouse::mouseItem = Mouse::cursor_ID;
    }
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