#include "map"
#include "array"
#include "Fire/fire.h"
#include "SDL2/SDL.h"
#include "ai_control.h"
#include "components.h"
#include "sinister_strike.h"
#include "ui.h"

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

  int SetStateAttack2(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    Action_Component::Set_State(action, Action_Component::attack2);
    return 0;
  }

  int Sinister_Strike(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    Sinister_Strike::Instant_Attack(zone, entity);
    return 0;
  }

  int Toggle_AI(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    AI::Turn_On();
    return 0;
  }

  int Tab_Target(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    User_Mouse_Input::Tab_Target(zone, entity);
    return 0;
  }

  int Bag_Toggle(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    UI::Bag_UI::Toggle_Bag();
    return 0;
  }

  int Menu_Toggle(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    if (UI_Spellbook::spellbook.b_isOpen || UI::bToggleCharacterUI) {
      UI::bToggleCharacterUI = false;
      UI_Spellbook::spellbook.b_isOpen = false;
      return 0;
    }
    Menu::Toggle();
    return 0;
  }

  int Pause_Toggle(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    Menu::Toggle();
    return 0;
  }

  int Mouse_On(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    SDL_SetRelativeMouseMode(SDL_FALSE);
    return 0;
  }

  int Mouse_Off(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    return 0;
  }

  int Toggle_Spellbook(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    UI_Spellbook::Toggle();
    return 0;
  }

  int Show_Items(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    Items::showGroundItems = true;
    return 0;
  }

  int Unshow_Items(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    Items::showGroundItems = false;
    return 0;
  }

  int Jump(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
    Action_Component::Set_State(action, Action_Component::jump);
    return 0;
  }

  Fire::castSpell Spells[] = {
      SetStateAttack2,
      Fire::Cast_Spell,
      Fire::Cast_Spell};

  std::map<SDL_Keycode, Fire::castSpell> keybinds = {
      {SDLK_1, SetStateAttack2},
      {SDLK_2, Sinister_Strike},
      {SDLK_3, Fire::Cast_Spell},
      {SDLK_9, Toggle_AI},
      {SDLK_TAB, Tab_Target},
      {SDLK_ESCAPE, Menu_Toggle},
      {SDLK_i, Bag_Toggle},
      {SDLK_p, Pause_Toggle},
      {SDLK_PLUS, Mouse_On},
      {SDLK_MINUS, Mouse_Off},
      {SDLK_l, Toggle_Spellbook},
      {SDLK_LALT, Show_Items},
      {SDLK_RALT, Show_Items},
      {SDLK_SPACE, Jump}
  };

  std::map<SDL_Keycode, Fire::castSpell> keyupKeybinds = {
      {SDLK_LALT, Unshow_Items},
      {SDLK_RALT, Unshow_Items}
  };

  std::map<int, Fire::castSpell> hotBarSpells = {
      {0, SetStateAttack2},
      {1, Fire::Cast_Spell},
      {2, Fire::Cast_Spell}};
  //  save the index to the button

  Fire::castSpell Update_Button() {
    return Fire::Cast_Spell;
  }

  void gfdgdfg(SDL_Keycode key) {
    hotBarSpells[key] = Update_Button();
  }
}// namespace Hotbar
