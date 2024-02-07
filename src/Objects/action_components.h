#pragma once
#include "item_components.h"

namespace Action_Component {

  enum Action_State {
    isStatic,
    idle,
    idle2,
    idle6,
    walk,
    walkBow,
    run,
    attack,
    attack2,
    attack2H_1,
    attack2H_2,
    attackPolearm,
    attackAOE,
    idle2H,
    idleCritical1,
    idleCritical2,
    jump,
    kick,
    interact,
    talk,
    run2H,
    walk2H,
    casting,
    cast,
    struck,
    block,
    evade,
    stunned,
    search,
    cleaning_object,
    idle_low_hp,
    idle_low_hp2,
    dying,
    dead,
    dead2,
    dead3,
    dead4,
    dead5,
    dead6,
    resting,
    ranged,
    ranged_bow1,
    idleBow,
    cheer,
    behavior,
    summoned,
    kneel,
    pray_kneeled,
    pray_standing,
    in_combat,
    in_combat2,
    climb,
    use_item,
    crouch
  };


  std::string Get_State_String(Action_State actionState) {
    switch (actionState) {

      case isStatic:
        return "isStatic";
      case idle:
        return "idle";
      case idle2:
        return "idle2";
      case idle6:
        return "idle6";
      case walk:
        return "walk";
      case walkBow:
        return "walkBow";
      case run:
        return "run";
      case attack:
        return "attack";
      case attack2:
        return "attack2";
      case attack2H_1:
        return "attack2H_1";
      case attack2H_2:
        return "attack2H_2";
      case attackPolearm:
        return "attackPolearm";
      case attackAOE:
        return "attackAOE";
      case idle2H:
        return "idle2H";
      case idleCritical1:
        return "idleCritical1";
      case idleCritical2:
        return "idleCritical2";
      case jump:
        return "jump";
      case kick:
        return "kick";
      case interact:
        return "interact";
      case talk:
        return "talk";
      case run2H:
        return "run2H";
      case walk2H:
        return "walk2H";
      case casting:
        return "casting";
      case cast:
        return "cast";
      case struck:
        return "struck";
      case block:
        return "block";
      case evade:
        return "evade";
      case stunned:
        return "stunned";
      case search:
        return "search";
      case cleaning_object:
        return "cleaning_object";
      case idle_low_hp:
        return "idle_low_hp";
      case idle_low_hp2:
        return "idle_low_hp2";
      case dying:
        return "dying";
      case dead:
        return "dead";
      case dead2:
        return "dead2";
      case dead3:
        return "dead3";
      case dead4:
        return "dead4";
      case dead5:
        return "dead5";
      case dead6:
        return "dead6";
      case resting:
        return "resting";
      case ranged:
        return "ranged";
      case ranged_bow1:
        return "ranged_bow1";
      case idleBow:
        return "idleBow";
      case cheer:
        return "cheer";
      case behavior:
        return "behavior";
      case summoned:
        return "summoned";
      case kneel:
        return "kneel";
      case pray_kneeled:
        return "pray_kneeled";
      case pray_standing:
        return "pray_standing";
      case in_combat:
        return "in_combat";
      case in_combat2:
        return "in_combat2";
      case climb:
        return "climb";
      case use_item:
        return "use_item";
      case crouch:
        return "crouch";
    }
    return std::to_string((int) actionState) + " state not found in enum";
  }

  enum Action_Progress {
    start,
    mid,
    last
  };

  struct Action {
    Action_State state = walk;
    Action_Progress frameState = start;
    int frame = 0;
    float frameTime = 0;
    Item_Component::Weapon_Type weaponType = Item_Component::Weapon_Type::axe;
  };

  void Set_State(Action &action, Action_State state) {
    if (state == walk)
      Utilities::Log("setting state to: " + Get_State_String(state));
    if (action.state == state) {
      return;
    }
    action.state = state;
    action.frameState = start;
    action.frameTime = 0;
    action.frame = 0;
  }

}// namespace Action_Component
