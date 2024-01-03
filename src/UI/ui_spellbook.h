#pragma once
#include "graphics.h"
#include "spell_data.h"
#include "ui_elements.h"
#include "sinister_strike.h"
#include "mouse_control.h"
#include "mouse_data.h"

#include "Life/life.h"
#include "Fire/fire.h"
#include "Ice/ice.h"
#include "lightning/lightning.h"
#include "lightning/lightningball.h"
#include "Nature/leafspell.h"
#include "Air/smoke.h"
#include "Arcane/arcane.h"
#include "Life/heal_self.h"
#include "Necromancy/necromancy.h"

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

  int PLACEHOLDER(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    return 0;
  }

  int Sinister_Strike(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    Sinister_Strike::Instant_Attack(zone, entity);
    return 0;
  }

  void Build_Spellbook() {
    spellbook.Skill_Trees[Spell_Data::cold][0] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][1] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][2] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][3] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][4] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][5] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][6] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][7] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][8] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][9] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][10] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::cold][11] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);

    spellbook.Skill_Trees[Spell_Data::earth][0] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][1] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][2] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][3] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][4] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][5] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][6] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][7] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][8] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][9] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][10] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::earth][11] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);

    spellbook.Skill_Trees[Spell_Data::death][0] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][1] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][2] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][3] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][4] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][5] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][6] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][7] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][8] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][9] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][10] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::death][11] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);

    spellbook.Skill_Trees[Spell_Data::life][0] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][1] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][2] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][3] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][4] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][5] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][6] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][7] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][8] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][9] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][10] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::life][11] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);

    spellbook.Skill_Trees[Spell_Data::warrior][0] = Spell_Data::Spell(Graphics::sinisterstrike, Sinister_Strike);
    spellbook.Skill_Trees[Spell_Data::warrior][1] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][2] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][3] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][4] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][5] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][6] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][7] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][8] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][9] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][10] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::warrior][11] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);

    spellbook.Skill_Trees[Spell_Data::archery][0] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][1] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][2] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][3] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][4] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][5] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][6] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][7] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][8] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][9] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][10] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);
    spellbook.Skill_Trees[Spell_Data::archery][11] = Spell_Data::Spell(Graphics::default_icon, PLACEHOLDER);

    spellbook.Skill_Trees[Spell_Data::fire][0] = Spell_Data::Spell(Graphics::fireball_icon, Fire::Cast_Spell);
    spellbook.Skill_Trees[Spell_Data::fire][1] = Spell_Data::Spell(Graphics::healself, Heal_Self::Heal_Self);
    spellbook.Skill_Trees[Spell_Data::fire][2] = Spell_Data::Spell(Graphics::healother, Life::Heal_Other);
    spellbook.Skill_Trees[Spell_Data::fire][3] = Spell_Data::Spell(Graphics::icebolt, Ice::Cast_Spell);
    spellbook.Skill_Trees[Spell_Data::fire][4] = Spell_Data::Spell(Graphics::chargedbolt, Lightning::Cast_Spell);
    spellbook.Skill_Trees[Spell_Data::fire][5] = Spell_Data::Spell(Graphics::lightningball, LightningBall::Cast_Spell);
    spellbook.Skill_Trees[Spell_Data::fire][6] = Spell_Data::Spell(Graphics::lightningstrike, Lightning::Cast_Spell2);
    spellbook.Skill_Trees[Spell_Data::fire][7] = Spell_Data::Spell(Graphics::leafspell, Nature::Cast_Spell);
    spellbook.Skill_Trees[Spell_Data::fire][8] = Spell_Data::Spell(Graphics::swirlingsmoke, Arrogate_Ghost::Cast);
    spellbook.Skill_Trees[Spell_Data::fire][9] = Spell_Data::Spell(Graphics::fire_1b_40, Fire::Fire);
    spellbook.Skill_Trees[Spell_Data::fire][10] = Spell_Data::Spell(Graphics::warp_effect, Arcane::Cast_Spell);
    spellbook.Skill_Trees[Spell_Data::fire][11] = Spell_Data::Spell(Graphics::death1, Summon_Demon::Cast);
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

  bool Get_SpellPage(entt::registry &zone, Component::Camera &camera, SDL_FRect &draw, bool &mouseHasItem, Spell_Data::Skill_Tree tree) {
    float xSpacing = 32.0f;
    float ySpacing = 32.0f;
    float x = draw.x;
    float y = draw.y;
    float i = 1;
    float w = 0;
    for (int j = 0; j < spellbook.Skill_Trees[tree].size(); ++j) {
      SDL_FRect renderRect = {x + 192.0f + (spellbook.Skill_Trees[tree][j].icon.renderRectSize.x * w) + (xSpacing * w), y + 128.0f, spellbook.Skill_Trees[tree][j].icon.renderRectSize.x, spellbook.Skill_Trees[tree][j].icon.renderRectSize.x};
      w++;
      if ((j + 1) % 3 == 0) {
        x = draw.x;
        y = draw.y + ((ySpacing + spellbook.Skill_Trees[tree][j].icon.renderRectSize.x) * i);
        i++;
        w = 0.0f;
      }
      renderRect = UI::Update_Scale(camera.scale, renderRect);

      if (Mouse::bRect_inside_Cursor(renderRect)) {
        Utilities::Log("picked up spell at spellbook index " + std::to_string(j));
//        mouseHasItem = true;
        Mouse_Struct::mouseData.tree = tree;
        Mouse_Struct::mouseData.index = j;
        Mouse_Struct::mouseData.type = Component::Icon_Type::spell;
//        Mouse::itemCurrentlyHeld = true;
        return true;
      }
    }
    return false;
  }

  bool Get_Spell(entt::registry &zone, Component::Camera &camera, bool &mouseHasItem) {
    if (Get_SpellPage(zone, camera, spellbook.panelRect, mouseHasItem, Spell_Data::Skill_Tree::fire)) {
      return true;
    }
    SDL_FRect page2 = spellbook.panelRect;
    page2.w /= 2;
    page2.x += page2.w - 64.0f;
    if (Get_SpellPage(zone, camera, page2, mouseHasItem, Spell_Data::Skill_Tree::warrior)) {
      return true;
    }
    return false;
  }

  void Draw_Spell_Page(entt::registry &zone, Component::Camera &camera, SDL_FRect &draw, Spell_Data::Skill_Tree tree) {
    float xSpacing = 32.0f;
    float ySpacing = 32.0f;
    float x = draw.x;
    float y = draw.y;
    float i = 1;
    float w = 0;
    for (int j = 0; j < spellbook.Skill_Trees[tree].size(); ++j) {
      SDL_FRect renderRect = {x + 192.0f + (spellbook.Skill_Trees[tree][j].icon.renderRectSize.x * w) + (xSpacing * w), y + 128.0f, spellbook.Skill_Trees[tree][j].icon.renderRectSize.x, spellbook.Skill_Trees[tree][j].icon.renderRectSize.x};
      w++;
      if ((j + 1) % 3 == 0) {
        x = draw.x;
        y = draw.y + ((ySpacing + spellbook.Skill_Trees[tree][j].icon.renderRectSize.x) * i);
        i++;
        w = 0.0f;
      }
      renderRect = UI::Update_Scale(camera.scale, renderRect);

      SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[tree][j].icon.pBackground, NULL, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[tree][j].icon.pTexture, NULL, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[tree][j].icon.pIconBorder, NULL, &renderRect);
      SDL_RenderCopyF(Graphics::renderer, spellbook.Skill_Trees[tree][j].icon.pIconRarityBorder, NULL, &renderRect);
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
    Draw_Spell_Page(zone, camera, draw, Spell_Data::Skill_Tree::fire);
    SDL_FRect page2 = spellbook.panelRect;
    page2.w /= 2;
    page2.x += page2.w - 64.0f;
    Draw_Spell_Page(zone, camera, page2, Spell_Data::Skill_Tree::warrior);
  }

  void Draw_Spellbook(entt::registry &zone, Component::Camera &camera) {
    if (spellbook.b_isOpen) {
      SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
      SDL_RenderCopyF(Graphics::renderer, spellbook.background, &spellbook.backgroundFrame, &draw);
      Draw_Pages(zone, camera, spellbook.panelRect);
    }
  }
}// namespace UI_Spellbook