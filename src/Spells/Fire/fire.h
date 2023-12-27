#pragma once
#include "spells.h"
#include "mouse_control.h"

namespace Fire {
  struct Cast_Data {
    int spellIndex = 0;
    const char *name = "fireball";
  };

  typedef int (*spells)(entt::registry &zone, entt::entity &entity, float &x, float &y);
  typedef int (*castSpell)(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y);

  int Fireball(entt::registry &zone, entt::entity &entity, float &x, float &y) {
    //get data from db
    Cast_Data castData;
    float castTime = 500.0f;

    //send to generic create
    zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, castData.name);
    return 1;
  }

  spells Fire_Spells[] = {Fireball};

  int Cast_Spell(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    if (action.state != Action_Component::casting && action.state != Action_Component::cast) {

      //read animation in from db?
      Action_Component::Set_State(action, Action_Component::casting);
      Fire_Spells[0](zone, entity, x, y);
    }
    return 0;
  }
}// namespace Fire