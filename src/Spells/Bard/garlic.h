#pragma once
#include "mouse_control.h"
#include "spells.h"

namespace Garlic {

  int Cast(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
    Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
    return 1;
  }

  int Create(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
//    if (zone.any_of<Component::Aura_Damage>(caster_ID)) {
//      zone.remove<Component::Aura_Damage>(caster_ID);
//      return 0;
//    }
//    zone.emplace_or_replace<Component::Aura_Damage>(caster_ID);
//    return 1;

    if (zone.any_of<Component::Damage_Over_Time>(caster_ID)) {
      zone.remove<Component::Damage_Over_Time>(caster_ID);
      return 0;
    }
    auto &debuffs = zone.emplace_or_replace<Component::Damage_Over_Time>(caster_ID);
    Component::DOT d;
    debuffs.debuffs.emplace_back(d);
    return 1;
  }

  int Hit(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    Spells::Spell_Hit_Effect(zone, state, caster_ID, position, direction, spellname, targetX, targetY);
    return 1;
  }

  int Cast_Spell(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    if (action.state != Action_Component::casting && action.state != Action_Component::cast) {

      //read animation in from db?
      Action_Component::Set_State(action, Action_Component::casting);
      float castTime = 500.0f;
      //send to generic create
      zone.emplace_or_replace<Spells::Cast_Effect>(entity, Cast, Create, Hit);
      zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, "", "", "");
      return 1;
    }
    return 0;
  }
}// namespace Fire