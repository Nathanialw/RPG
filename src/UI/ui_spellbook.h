#pragma once
#include "graphics.h"
#include "ui_elements.h"

namespace UI_Spellbook {

  enum Skill_Tree {
    fire,
    cold,
    warrior,
    archery,
    life,
    death,
    SIZE
  };

  struct Spellbook {
    std::array<std::array<entt::entity, 12>, Skill_Tree::SIZE> Skill_Trees;
    bool b_isOpen = false;
    SDL_FRect panelRect;
    SDL_Rect backgroundFrame = {0, 0, 1137, 700};
    SDL_Texture *background = NULL;
    SDL_Rect leftPageFrame = {0, 0, 1137, 700};
    SDL_Texture *leftPage = NULL;
    SDL_Rect rightPageFrame = {0, 0, 1137, 700};
    SDL_Texture *rightPage = NULL;
    Skill_Tree currentTab = fire;
    entt::entity spell;
  };

  Spellbook spellbook;

  void Update_Position(SDL_FRect &frameRect) {
    SDL_Rect rect = Utilities::SDL_FRect_To_SDL_Rect(frameRect);
    frameRect = UI::Center_Rect(rect);
    int w;
    int h;
    SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
    frameRect.y = (float) h - frameRect.h;
  }

  entt::entity Create_Spell(entt::registry &zone) {
    auto spell = zone.create();
    //read db

    //load texture

    auto &icon = zone.emplace<Component::Icon>(spell, Graphics::emptyBagIcon, Graphics::fireball_explosion_0, Item_Component::rarityBorder[Item_Component::Rarity::common], Graphics::bagSlotBorder);
    zone.emplace<Component::Position>(spell);
    icon.clipSprite = {0, 0, 256, 256};
    icon.clipIcon = {0, 0, 256, 256};
    icon.renderRectSize = {64.0f, 64.0f};
    icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};
    auto &mouseItem = zone.emplace_or_replace<Component::On_Mouse>(spell);
    mouseItem.type = Component::Icon_Type::spell;
    return spell;
  }

  void Build_Spellbook(entt::registry &zone) {
    for (auto &tree: spellbook.Skill_Trees) {
      for (auto &spell: tree) {
        spell = Create_Spell(zone);
      }
    }
  }

  //    run once the camera is attached to a unit
  void Update_Position() {
    spellbook.panelRect = UI::Center_Rect(spellbook.backgroundFrame);
  }

  void Init_UI(entt::registry &zone) {
    Update_Position();
    spellbook.background = Graphics::spellbook;
    Build_Spellbook(zone);
  }

  void Close_Spellbook() {
    if (spellbook.b_isOpen) {
      spellbook.b_isOpen = false;
    }
  }

  void Toggle() {
    if (spellbook.b_isOpen) {
      spellbook.b_isOpen = false;
    } else {
      spellbook.b_isOpen = true;
    }
  }

  bool Get_SpellPage(entt::registry &zone, Component::Camera &camera, SDL_FRect &draw, bool &mouseHasItem) {
    float xSpacing = 32.0f;
    float ySpacing = 32.0f;
    float x = draw.x;
    float y = draw.y;
    float i = 1;
    float w = 0;
    for (int j = 0; j < spellbook.Skill_Trees[fire].size(); ++j) {
      auto &icon = zone.get<Component::Icon>(spellbook.Skill_Trees[fire][j]);
      SDL_FRect renderRect = {x + 192.0f + (icon.renderRectSize.x * w) + (xSpacing * w), y + 128.0f, icon.renderRectSize.x, icon.renderRectSize.x};
      w++;
      if ((j + 1) % 3 == 0) {
        x = draw.x;
        y = draw.y + ((ySpacing + icon.renderRectSize.x) * i);
        i++;
        w = 0.0f;
      }
      renderRect = UI::Update_Scale(camera.scale, renderRect);

      if (Mouse::bRect_inside_Cursor(renderRect)) {
        mouseHasItem = true;
        Mouse::mouseItem = spellbook.Skill_Trees[fire][i];
        auto &mouseItem = zone.emplace_or_replace<Component::On_Mouse>(spellbook.Skill_Trees[fire][i]);
        mouseItem.type = Component::Icon_Type::spell;
        return true;
      }
    }
    return false;
  }

  bool Get_Spell(entt::registry &zone, Component::Camera &camera, bool &mouseHasItem) {
    if (Get_SpellPage(zone, camera, spellbook.panelRect, mouseHasItem)) {
      return true;
    }
    SDL_FRect page2 = spellbook.panelRect;
    page2.w /= 2;
    page2.x += page2.w - 64.0f;
    if (Get_SpellPage(zone, camera, page2, mouseHasItem)) {
      return true;
    }
    return false;
  }

  void Draw_Spell_Page(entt::registry &zone, Component::Camera &camera, SDL_FRect &draw) {
    float xSpacing = 32.0f;
    float ySpacing = 32.0f;
    float x = draw.x;
    float y = draw.y;
    float i = 1;
    float w = 0;
    for (int j = 0; j < spellbook.Skill_Trees[fire].size(); ++j) {
      auto &icon = zone.get<Component::Icon>(spellbook.Skill_Trees[fire][j]);
      SDL_FRect renderRect = {x + 192.0f + (icon.renderRectSize.x * w) + (xSpacing * w), y + 128.0f, icon.renderRectSize.x, icon.renderRectSize.x};
      w++;
      if ((j + 1) % 3 == 0) {
        x = draw.x;
        y = draw.y + ((ySpacing + icon.renderRectSize.x) * i);
        i++;
        w = 0.0f;
      }
      renderRect = UI::Update_Scale(camera.scale, renderRect);

      SDL_RenderCopyF(Graphics::renderer, icon.pTexture, &icon.clipIcon, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, icon.pIconRarityBorder, &icon.clipIcon, &renderRect);
    }
  }

  bool Check_Spellbook(Component::Camera &camera) {
    SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
    if (spellbook.b_isOpen && Mouse::bRect_inside_Cursor(draw)) {
      return true;
    }
    return false;
  }

  void Draw_Pages(entt::registry &zone, Component::Camera &camera, SDL_FRect &draw) {
    Draw_Spell_Page(zone, camera, draw);
    SDL_FRect page2 = spellbook.panelRect;
    page2.w /= 2;
    page2.x += page2.w - 64.0f;
    Draw_Spell_Page(zone, camera, page2);
  }

  void Draw_Spellbook(entt::registry &zone, Component::Camera &camera) {
    if (spellbook.b_isOpen) {
      SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
      SDL_RenderCopyF(Graphics::renderer, spellbook.background, &spellbook.backgroundFrame, &draw);
      Draw_Pages(zone, camera, spellbook.panelRect);
    }
  }
}// namespace UI_Spellbook