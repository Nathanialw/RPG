#pragma once
#include "spells.h"
#include "mouse_control.h"

namespace Lightning {
  struct Cast_Data {
    int spellIndex = 0;
    const char *name = "chargedbolt";
  };


  int chargedbolt(entt::registry &zone, entt::entity &entity, float &x, float &y, const char *name) {
    //get data from db
    float castTime = 500.0f;

    //send to generic create
    zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, name);
    return 1;
  }

  int spell_bluetop_1(entt::registry &zone, entt::entity &entity, float &x, float &y, const char *name) {
    //get data from db
    float castTime = 500.0f;

    //send to generic create
    zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, name);
    return 1;
  }


  Spells::spells Fire_Spells[] = {chargedbolt, spell_bluetop_1};

  int Cast_Spell(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    if (action.state != Action_Component::casting && action.state != Action_Component::cast) {

      //read animation in from db?
      Action_Component::Set_State(action, Action_Component::casting);
      Fire_Spells[0](zone, entity, x, y, "chargedbolt");
    }
    return 0;
  }

  int Cast_Spell2(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    if (action.state != Action_Component::casting && action.state != Action_Component::cast) {

      //read animation in from db?
      Action_Component::Set_State(action, Action_Component::casting);
      Fire_Spells[1](zone, entity, x, y, "spell_bluetop_1");
    }
    return 0;
  }
}// namespace Fire
