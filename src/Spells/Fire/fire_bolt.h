#pragma once
#include "mouse_control.h"
#include "spells.h"

namespace Fire_Bolt {

  int Template(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {

    return 1;
  }

  int Fire_Cast(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
    Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
    return 1;
  }

  int Fireball(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
    Spells::Create_Spell(zone, state, caster_ID, position, direction, hitEffect, casting, targetX, targetY);
    return 1;
  }

  int Fireball_Hit(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    Spells::Create_Explosion(zone, position.x, position.y);
    return 1;
  }

  int Cast_Spell(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    if (action.state != Action_Component::casting && action.state != Action_Component::cast) {

      //read animation in from db?
      Action_Component::Set_State(action, Action_Component::casting);
      float castTime = 500.0f;
      //send to generic create
      zone.emplace_or_replace<Spells::Cast_Effect>(entity, Fire_Cast, Fireball, Fireball_Hit);
      zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, "fireball", "", "fire_1b_40");
      return 1;
    }
    return 0;
  }

  int Fire(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    if (action.state != Action_Component::casting && action.state != Action_Component::cast) {

      //read animation in from db?
      Action_Component::Set_State(action, Action_Component::casting);
      float castTime = 500.0f;
      zone.emplace_or_replace<Spells::Cast_Effect>(entity, Fire_Cast, Fireball);
      zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, "", "fire_1b_40", "");
    }
    return 0;
  }
}// namespace Fire_Bolt