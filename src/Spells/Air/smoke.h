#pragma once
#include "Input/mouse_control.h"
#include "spells.h"

namespace Air {
  struct Cast_Data {
    int spellIndex = 0;
    const char *name = "";
  };


  int swirlingsmoke(entt::registry &zone, entt::entity &entity, float &x, float &y, const char *name) {
    //get data from db
    Cast_Data castData;
    float castTime = 500.0f;

    //send to generic create
    zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, name);
    return 1;
  }

  Spells::spells Fire_Spells[] = {swirlingsmoke};

  int Cast_Spell(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    if (action.state != Action_Component::casting && action.state != Action_Component::cast) {

      //read animation in from db?
      Action_Component::Set_State(action, Action_Component::casting);
      Fire_Spells[0](zone, entity, x, y, "swirlingsmoke");
    }
    return 0;
  }
}// namespace Air
