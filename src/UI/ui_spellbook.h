#pragma once
#include "graphics.h"
#include "spell_data.h"
#include "ui_elements.h"

namespace UI_Spellbook {

  struct Spellbook {
    std::array<std::array<Spell_Data::Spell, 12>, (int) Spell_Data::SIZE> Skill_Trees;
    bool b_isOpen = false;
    SDL_FRect panelRect;
    SDL_Rect backgroundFrame = {0, 0, 1137, 700};
    SDL_Texture *background = NULL;
    SDL_Rect leftPageFrame = {0, 0, 1137, 700};
    SDL_Texture *leftPage = NULL;
    SDL_Rect rightPageFrame = {0, 0, 1137, 700};
    SDL_Texture *rightPage = NULL;
    Spell_Data::Skill_Tree currentTab = Spell_Data::fire;
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

  Spell_Data::Spell Create_Spell() {
    //read db
    Spell_Data::Spell spell(Graphics::default_icon, Fire::Cast_Spell);
    //load texture
    return spell;
  }

  void Build_Spellbook() {
    for (auto &tree: spellbook.Skill_Trees) {
      for (auto &spell: tree) {
        spell = Create_Spell();
      }
    }
  }

  //    run once the camera is attached to a unit
  void Update_Position() {
    spellbook.panelRect = UI::Center_Rect(spellbook.backgroundFrame);
  }

  void Init_UI() {
    Update_Position();
    spellbook.background = Graphics::spellbook;
    Build_Spellbook();
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
    for (int j = 0; j < spellbook.Skill_Trees[Spell_Data::fire].size(); ++j) {
      SDL_FRect renderRect = {x + 192.0f + (spellbook.Skill_Trees[Spell_Data::fire][j].icon.renderRectSize.x * w) + (xSpacing * w), y + 128.0f, spellbook.Skill_Trees[Spell_Data::fire][j].icon.renderRectSize.x, spellbook.Skill_Trees[Spell_Data::fire][j].icon.renderRectSize.x};
      w++;
      if ((j + 1) % 3 == 0) {
        x = draw.x;
        y = draw.y + ((ySpacing + spellbook.Skill_Trees[Spell_Data::fire][j].icon.renderRectSize.x) * i);
        i++;
        w = 0.0f;
      }
      renderRect = UI::Update_Scale(camera.scale, renderRect);

      if (Mouse::bRect_inside_Cursor(renderRect)) {
        mouseHasItem = true;
        Mouse::ss.index = i;
        Mouse::ss.type = Component::Icon_Type::spell;
        Mouse::itemCurrentlyHeld = true;
        Mouse::ss.index = i;
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
    for (int j = 0; j < spellbook.Skill_Trees[Spell_Data::fire].size(); ++j) {
      SDL_FRect renderRect = {x + 192.0f + (spellbook.Skill_Trees[Spell_Data::fire][j].icon.renderRectSize.x * w) + (xSpacing * w), y + 128.0f, spellbook.Skill_Trees[Spell_Data::fire][j].icon.renderRectSize.x, spellbook.Skill_Trees[Spell_Data::fire][j].icon.renderRectSize.x};
      w++;
      if ((j + 1) % 3 == 0) {
        x = draw.x;
        y = draw.y + ((ySpacing + spellbook.Skill_Trees[Spell_Data::fire][j].icon.renderRectSize.x) * i);
        i++;
        w = 0.0f;
      }
      renderRect = UI::Update_Scale(camera.scale, renderRect);

      SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[Spell_Data::fire][j].icon.pTexture, &spellbook.Skill_Trees[Spell_Data::fire][j].icon.clipIcon, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[Spell_Data::fire][j].icon.pIconRarityBorder, &spellbook.Skill_Trees[Spell_Data::fire][j].icon.clipIcon, &renderRect);
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