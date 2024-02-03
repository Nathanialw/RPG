#pragma once
#include "components.h"
#include "entt/entt.hpp"
#include "spells.h"

namespace Metamorphosis {

  int Cast(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
    if (zone.any_of<Rendering_Components::Metamorphosis>(caster_ID)) {
      return 0;
    }
    Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
    return 1;
  }

  int Create(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
    auto &metamorphosis = zone.emplace_or_replace<Rendering_Components::Metamorphosis>(caster_ID);
    metamorphosis.duration = 10000;
    metamorphosis.unit = {"demons", 0};
    metamorphosis.spellEffect = "Effects02";
    return 1;
  }

  int Hit(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    //    Spells::Spell_Hit_Effect(zone, state, caster_ID, position, direction, spellname, targetX, targetY);
    return 1;
  }

  int Cast_Spell(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Action_Component::Set_State(action, Action_Component::casting);
    float castTime = 500.0f;

    zone.emplace_or_replace<Spells::Cast_Effect>(entity, Cast, Create, Hit);
    zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, "", "Effects02", "");
    return 1;
  }
}// namespace Metamorphosis