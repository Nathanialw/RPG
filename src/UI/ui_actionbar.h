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

  Spell_Data::Spell Create_Spell() {
    Spell_Data::Spell spell(Graphics::fireball_explosion_0);
    return spell;
  }

  void Create_Action_Bar() {
    actionBar.defaultSlot = Create_Spell();
    for (int i = 0; i < actionBar.actionBar.spell.size(); i++) {
      actionBar.actionBar.spell[i] = Create_Spell();
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
      auto &icon = actionBar.actionBar.spell[i].icon;
      SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};
      if (Mouse::bRect_inside_Cursor(renderRect)) {
        if (Mouse::ss.type == Component::Icon_Type::spell) {

          actionBar.actionBar.spell[i] = UI_Spellbook::spellbook.Skill_Trees[Spell_Data::fire][Mouse::ss.index];
          Hotbar_Structs::keybinds[actionBar.actionBar.hotkey[i]] = actionBar.actionBar.spell[i].cast;

          Mouse::itemCurrentlyHeld = false;
          Mouse::ss.type = Component::Icon_Type::none;
          Mouse::ss.index = -1;
          Mouse::mouseItem = Mouse::cursor_ID;
          return;
        } else {
          Mouse::itemCurrentlyHeld = false;
          Mouse::ss.type = Component::Icon_Type::none;
          Mouse::ss.index = -1;
          Mouse::mouseItem = Mouse::cursor_ID;
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

          Mouse::ss.index = i;
          actionBar.actionBar.spell[i] = actionBar.defaultSlot;
          Mouse::ss.type = Component::Icon_Type::spell;
          Mouse::itemCurrentlyHeld = true;

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
    Mouse::ss.type = Component::Icon_Type::none;
    Mouse::ss.index = -1;
  }

  void Render_Action_Bar(entt::registry &zone, int &state, Component::Camera &camera) {
    SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBar.actionBarFrame);

    for (int i = 0; i < actionBar.actionBar.spell.size(); i++) {
      auto &icon = actionBar.actionBar.spell[i].icon;
      SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};
      SDL_RenderCopyF(Graphics::renderer, icon.pBackground, &icon.clipIcon, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pTexture, &icon.clipIcon, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pIconRarityBorder, &icon.clipIcon, &renderRect);
    }
  }
}// namespace Action_Bar