#pragma once
#include "components.h"
#include "entt/entt.hpp"
#include "spells.h"

namespace Heal_Self {

  int Cast(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
    Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
    return 1;
  }

  int Create(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
    auto &health = zone.get<Component::Health>(caster_ID);
    health.currentHealth += 10;
    if (health.currentHealth > health.maxHealth) {
      health.currentHealth = health.maxHealth;
    }
    return 1;
  }

  int Hit(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    Spells::Spell_Hit_Effect(zone, state, caster_ID, position, direction, spellname, targetX, targetY);
    return 1;
  }

  int Heal_Self(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    //get data from db
    Action_Component::Set_State(action, Action_Component::pray_standing);
    float castTime = 500.0f;

    zone.emplace_or_replace<Spells::Cast_Effect>(entity, Cast, Create, Hit);
    zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, nullptr, "pipo-mapeffect013a", "BBLBubbleEllipseYellow");
    return 1;
  }
}// namespace Heal_Self