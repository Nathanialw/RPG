#pragma once
#include "components.h"
#include "entt/entt.hpp"
#include "spells.h"

namespace Life {

  int Cast(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
    SDL_FRect mouseRect = Utilities::Get_FRect_From_Point_Radius(Mouse::cursorRadius, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
    Quad_Tree::Entity_Data targetData = Quad_Tree::Entity_vs_Mouse_Collision(zone, mouseRect, state);
    if (targetData.b) {
      Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
      casting.target_ID = targetData.entity_ID;
    }
    else {
      return 0;
    }
    return 1;
  }

  int Create(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
    //get data from db
    auto &health = zone.get<Component::Health>(casting.target_ID);
    health.currentHealth += 10;
    if (health.currentHealth > health.maxHealth) {
      health.currentHealth = health.maxHealth;
    }
    auto &targetPosition = zone.get<Component::Position>(casting.target_ID);
    Spells::Spell_Hit_Effect(zone, state, casting.target_ID, targetPosition, direction, casting.hit, targetX, targetY);
    return 1;
  }

  int Hit(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    //    Spells::Spell_Hit_Effect(zone, state, caster_ID, position, direction, spellname, targetX, targetY);
    return 1;
  }

  int Heal_Other(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    Action_Component::Set_State(action, Action_Component::casting);
    float castTime = 500.0f;

    zone.emplace_or_replace<Spells::Cast_Effect>(entity, Cast, Create, Hit);
    zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, "", "wings", "BBLBubbleEllipseYellow");
    return 0;
  }
}// namespace Life