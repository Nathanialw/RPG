#pragma once
#include "Audio/effects.h"
#include "Fire/fire.h"
#include "Fog_Of_War/fog_of_war.h"
#include "Maps/World/cave.h"
#include "SDL2/SDL.h"
#include "UI/ui_info.h"
#include "ai_control.h"
#include "array"
#include "components.h"
#include "hotbar_structs.h"
#include "interface.h"
#include "map"
#include "sinister_strike.h"
#include "string"
#include "ui.h"
#include "ui_actionbar.h"
/*
 * make an array of functions for everything that can go onto the hotbar, ie skills
 *
 * save them all into any array
 *  ie, spell[tree][skill]
 *
 *  use a pointer to this array to call the spell
 *
 * when you play a spell into a skill bar it save the skill/tree
 * when you click the spell it recalls the skill/tree
 * then uses them to call the function from the array
 *
 * */

namespace Hotbar {
  //  std::unordered_map<SDL_Keycode, int> hotbar;

  struct Cast_Data {
    int num = 0;
    std::string name = "none";
  };

  Cast_Data hotbar[12];
  typedef int (*ssSpells)(int a);

  int function(int a) { return a; }
  int functionTimesTwo(int a) { return a * 2; }
  int functionDivideByTwo(int a) { return a / 2; }

  ssSpells Fire_Spells[] = {function,
                            functionTimesTwo,
                            functionDivideByTwo};

  void Call_Functions() {
    for (int i = 0; i < 3; ++i) {
      std::cout << Fire_Spells[i](8) << std::endl;
    }
  }

  int SetStateAttack2(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Action_Component::Set_State(action, Action_Component::attack2);
    return 0;
  }

  int Sinister_Strike(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Sinister_Strike::Instant_Attack(zone, entity);
    return 0;
  }

  int Toggle_AI(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    AI::Turn_On();
    return 0;
  }

  int Tab_Target(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    User_Mouse_Input::Tab_Target(zone, entity);
    return 0;
  }

  int Bag_Toggle(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Bag_UI::Toggle_Bag();
    return 0;
  }

  int Info_Toggle(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    UI_Info::Info_Toggle();
    return 0;
  }

  int Menu_Toggle(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    auto view = zone.view<Component::Selected>();
    if (Mouse_Struct::mouseData.type == Component::Icon_Type::building) {
      zone.emplace_or_replace<Component::Destroyed>(Mouse::mouseData.mouseItem);
      Mouse::Set_Cursor_As_Cursor(zone);
      return 0;
    } else if (UI_Spellbook::spellbook.b_isOpen || Bag_UI::bToggleCharacterUI || UI_Info::spellbook.b_isOpen || Loot_Panel::lootPanel.items || Mouse::bLeft_Mouse_Pressed || UI_Frames::topFrame.open || !view.empty()) {
      Mouse::bLeft_Mouse_Pressed = false;
      zone.clear<Component::Selected>();
      Loot_Panel::Close();
      Bag_UI::bToggleCharacterUI = false;
      UI_Spellbook::spellbook.b_isOpen = false;
      UI_Info::spellbook.b_isOpen = false;
      Game_Menu_Control::Close();
      return 0;
    }
    Menu::Toggle();
    return 0;
  }

  bool autoRun = false;
  int Auto_Run(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    autoRun = true;
    Mouse::bRight_Mouse_Pressed = true;
    return 0;
  }

  int Pause_Toggle(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Menu::Toggle();
    return 0;
  }

  int Mouse_On(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    SDL_SetRelativeMouseMode(SDL_FALSE);
    return 0;
  }

  int Mouse_Off(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    return 0;
  }

  int Toggle_Spellbook(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    UI_Spellbook::Toggle();
    return 0;
  }

  int Show_Items(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Items::showGroundItems = true;
    return 0;
  }

  int Unshow_Items(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Items::showGroundItems = false;
    return 0;
  }

  int Jump(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Action_Component::Set_State(action, Action_Component::jump);
    return 0;
  }

  int Surface(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Cave::Surface(index);
    return 0;
  }

  int Next_Page(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    UI_Spellbook::Next_Page();
    return 0;
  }

  int Previous_Page(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    UI_Spellbook::Previous_Page();
    return 0;
  }

  int PLACEHOLDER(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    return 0;
  }

  int Zoom_In(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Interface::Update_Zoom(zone, 1);
    return 0;
  }

  int Zoom_Out(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Interface::Update_Zoom(zone, -1);
    return 0;
  }

  int Sight_Increase(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Fog_Of_War::setFogOfWar = false;
    Fog_Of_War::lightRadius++;
    if (Fog_Of_War::lightRadius >= 15) Fog_Of_War::lightRadius = 15;
    return 0;
  }

  int Sight_Decrease(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Fog_Of_War::setFogOfWar = false;
    Fog_Of_War::lightRadius--;
    if (Fog_Of_War::lightRadius <= 1) Fog_Of_War::lightRadius = 1;
    return 0;
  }

  int Destroy_Entity(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    auto view = zone.view<Component::Selected>();
    for (auto &entityID: view) {
      zone.emplace_or_replace<Component::Destroyed>(entityID);
    }

    return 0;
  }

  int Place_Building(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Build::Place_Building(zone, state);
    return 0;
  }

  void Init_Hotbar() {
    Hotbar_Structs::keybinds = {
        {SDLK_1, Action_Bar::actionBar.actionBar.spell[0].cast},
        {SDLK_2, Action_Bar::actionBar.actionBar.spell[1].cast},
        {SDLK_3, Action_Bar::actionBar.actionBar.spell[2].cast},
        {SDLK_4, Action_Bar::actionBar.actionBar.spell[3].cast},
        {SDLK_5, Action_Bar::actionBar.actionBar.spell[5].cast},
        {SDLK_6, Action_Bar::actionBar.actionBar.spell[6].cast},
        {SDLK_7, Action_Bar::actionBar.actionBar.spell[7].cast},
        {SDLK_8, Action_Bar::actionBar.actionBar.spell[8].cast},
        {SDLK_9, Action_Bar::actionBar.actionBar.spell[9].cast},
        {SDLK_TAB, Tab_Target},
        {SDLK_ESCAPE, Menu_Toggle},
        {SDLK_r, Auto_Run},
        {SDLK_i, Info_Toggle},
        {SDLK_b, Bag_Toggle},
        {SDLK_p, Pause_Toggle},
        {SDLK_LEFTBRACKET, Mouse_On},
        {SDLK_RIGHTBRACKET, Mouse_Off},
        {SDLK_MINUS, Zoom_Out},
        {SDLK_EQUALS, Zoom_In},
        {SDLK_l, Toggle_Spellbook},
        {SDLK_k, Toggle_AI},
        {SDLK_o, Surface},
        {SDLK_LALT, Show_Items},
        {SDLK_RALT, Show_Items},
        {SDLK_SPACE, Jump},
        {SDLK_PERIOD, Next_Page},
        {SDLK_COMMA, Previous_Page},
        {SDLK_UP, Sight_Increase},
        {SDLK_DOWN, Sight_Decrease},
        {SDLK_DELETE, Destroy_Entity},
        {SDLK_RETURN, Place_Building},
        {SDLK_RETURN2, Place_Building},
    };
  }

  std::map<SDL_Keycode, Spells::castSpell> keyupKeybinds = {
      {SDLK_LALT, Unshow_Items},
      {SDLK_RALT, Unshow_Items}};

}// namespace Hotbar
