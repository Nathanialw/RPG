#pragma once
#include "entt/entt.hpp"
#include "graphics.h"
#include "hotbar_structs.h"
#include "item_components.h"
#include "ui_elements.h"
#include "ui_spellbook.h"
#include "world.h"

namespace Action_Bar {
  struct On_Spellbar {
  };

  struct Spell_Bar {
    //    std::array<entt::entity, 10> spell;
    std::array<Spell_Data::Spell, 10> spell;
    std::array<SDL_Keycode, 10> hotkey;
    SDL_FRect actionBarFrame = {0.0f, 0, 640.0f, 64.0f};
  };

  struct Action_Bar {
    Spell_Bar actionBar;
    Spell_Data::Spell defaultSlot;
  };
  Action_Bar actionBar;

  void Update_Position(SDL_FRect &frameRect) {
    SDL_Rect rect = Utilities::SDL_FRect_To_SDL_Rect(frameRect);
    frameRect = UI::Center_Rect(rect);
    int w;
    int h;
    SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
    frameRect.y = (float) h - frameRect.h;
  }

  int PLACEHOLDER(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    return 0;
  }

  void Create_Action_Bar() {
    actionBar.defaultSlot = Spell_Data::Spell(Graphics::emptyBagIcon, PLACEHOLDER);
    for (int i = 0; i < actionBar.actionBar.spell.size(); i++) {
      actionBar.actionBar.spell[i] = actionBar.defaultSlot;
    }
    Update_Position(actionBar.actionBar.actionBarFrame);
    actionBar.actionBar.hotkey[0] = SDLK_1;
    actionBar.actionBar.hotkey[1] = SDLK_2;
    actionBar.actionBar.hotkey[2] = SDLK_3;
    actionBar.actionBar.hotkey[3] = SDLK_4;
    actionBar.actionBar.hotkey[4] = SDLK_5;
    actionBar.actionBar.hotkey[5] = SDLK_6;
    actionBar.actionBar.hotkey[6] = SDLK_7;
    actionBar.actionBar.hotkey[7] = SDLK_8;
    actionBar.actionBar.hotkey[8] = SDLK_9;
    actionBar.actionBar.hotkey[9] = SDLK_0;
  }

  bool Mouse_Inside_Actionbar(Component::Camera &camera, int &state) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBar.actionBarFrame);
    if (Mouse::bRect_inside_Cursor(renderBarFrame)) {
      return true;
    }
    return false;
  }

  void Set_Mouse_Spell_On_Actionbar(entt::registry &zone, int &state, Component::Camera &camera) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBar.actionBarFrame);

    for (int i = 0; i < actionBar.actionBar.spell.size(); i++) {
      SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};
      if (Mouse::bRect_inside_Cursor(renderRect)) {
        if (Mouse_Struct::mouseData.type == Component::Icon_Type::spell) {

          SDL_Texture *texture = actionBar.actionBar.spell[i].icon.pTexture;
          Spell_Data::Skill_Tree tree = actionBar.actionBar.spell[i].tree;
          int index = actionBar.actionBar.spell[i].index;
          actionBar.actionBar.spell[i] = UI_Spellbook::spellbook.Skill_Trees[Mouse_Struct::mouseData.tree][Mouse_Struct::mouseData.index];
          actionBar.actionBar.spell[i].tree = Mouse_Struct::mouseData.tree;
          actionBar.actionBar.spell[i].index = Mouse_Struct::mouseData.index;

          Hotbar_Structs::keybinds[actionBar.actionBar.hotkey[i]] = actionBar.actionBar.spell[i].cast;

          if (texture == actionBar.defaultSlot.icon.pTexture) {
            Mouse_Struct::mouseData.type = Component::Icon_Type::none;
            Mouse_Struct::mouseData.index = -1;
            return;
          }
//          swap
          Mouse_Struct::mouseData.tree = tree;
          Mouse_Struct::mouseData.index = index;
          return;
        } else {
//          Mouse::itemCurrentlyHeld = false;
          Mouse_Struct::mouseData.type = Component::Icon_Type::none;
          Mouse_Struct::mouseData.index = -1;
          //          clear the mouse item
        }
      }
    }
  }

  void Get_Mouse_Spell_From_Actionbar(entt::registry &zone, int &state, Component::Camera &camera) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBar.actionBarFrame);

    for (int i = 0; i < actionBar.actionBar.spell.size(); i++) {
      SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};
      if (Mouse::bRect_inside_Cursor(renderRect)) {
        if (actionBar.actionBar.spell[i].icon.pTexture != actionBar.defaultSlot.icon.pTexture) {

          Mouse_Struct::mouseData.tree = actionBar.actionBar.spell[i].tree;
          Mouse_Struct::mouseData.index = actionBar.actionBar.spell[i].index;
          actionBar.actionBar.spell[i] = actionBar.defaultSlot;
          Mouse_Struct::mouseData.type = Component::Icon_Type::spell;
//          Mouse::itemCurrentlyHeld = true;

          Hotbar_Structs::keybinds[actionBar.actionBar.hotkey[i]] = actionBar.actionBar.spell[i].cast;
          return;
        }
        //          clear the mouse item
      }
    }
  }

  void Clear_Spell_On_Mouse(entt::registry &zone) {
    if (Mouse::mouseItem == Mouse::cursor_ID) {
      Utilities::Log("objectID on mouse is the same as the cursorID same");
    }
    Mouse_Struct::mouseData.type = Component::Icon_Type::none;
    Mouse_Struct::mouseData.index = -1;
  }

  void Render_Action_Bar(entt::registry &zone, int &state, Component::Camera &camera) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBar.actionBarFrame);

    for (int i = 0; i < actionBar.actionBar.spell.size(); i++) {
      auto &icon = actionBar.actionBar.spell[i].icon;
      SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};
      SDL_RenderCopyF(Graphics::renderer, icon.pBackground, NULL, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pTexture, NULL, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pIconBorder, NULL, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pIconRarityBorder, NULL, &renderRect);
    }
  }
}// namespace Action_Bar