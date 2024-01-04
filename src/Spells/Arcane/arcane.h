#pragma once
#include "spells.h"
#include "mouse_control.h"

namespace Arcane {

  int Cast(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
    Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
    return 1;
  }

  int Create (entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
    auto &body = zone.get<Component::Body>(caster_ID);
    b2Vec2 p = {targetX, targetY};
    body.body->SetTransform(p, 0);
    b2Vec2 impulse = {0.1, 0.1};
    body.body->ApplyForce(impulse, body.body->GetWorldCenter(), true);
    return 1;
  }

  int Hit (entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
//    Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
    return 1;
  }

  int Cast_Spell(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    if (action.state != Action_Component::casting && action.state != Action_Component::cast) {
      float castTime = 500.0f;

      //read animation in from db?
      Action_Component::Set_State(action, Action_Component::casting);
      zone.emplace_or_replace<Spells::Cast_Effect>(entity, Cast, Create);
      zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, "", "teleport", "", false);
    }
    return 0;
  }
}// namespace Fire
