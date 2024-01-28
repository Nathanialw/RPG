#pragma once
#include "components.h"
#include "entt/entt.hpp"
#include "spells.h"

namespace Raise_Zombie {

  int Cast(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
    SDL_FRect mouseRect = Utilities::Get_FRect_From_Point_Radius(Mouse::cursorRadius, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
    Quad_Tree::Entity_Data targetData = Quad_Tree::Entity_vs_Mouse_Collision(zone, mouseRect, state);
    if (targetData.b) {
      if (zone.get<Component::Entity_Type>(targetData.entity_ID) == Component::Entity_Type::unit) {
        auto &alive = zone.get<Component::Alive>(targetData.entity_ID);
        if (!alive.bIsAlive) {
          Component::Position targetPosition = {Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse};
          Spells::Spell_Cast_Effect(zone, state, caster_ID, targetPosition, direction, casting.effect, targetX, targetY);
          casting.target_ID = targetData.entity_ID;
          return 1;
        }
      }
      return 0;
    }
    return 0;
  }

  int Create(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
    db::Unit_Data data = Game_Objects_Lists::units["zombies"][0];
    Component::Unit_Index unitIndex = {"zombies", 0};

    Social_Component::Summon summon;
    summon.relationships = zone.get<Social_Component::Relationships>(caster_ID);
    summon.summon = true;
    summon.race = zone.get<Social_Component::Race>(caster_ID);
    Component::Position targetPosition = {casting.x, casting.y};
    Create_Entities::Create_Entity(zone, state, casting.x, casting.y, data, false, summon, unitIndex);

    auto &rect = zone.get<Component::Interaction_Rect>(casting.target_ID);
    zone.emplace_or_replace<Component::Destroyed>(casting.target_ID);
    zone.emplace_or_replace<Component::Remove_From_Object_Tree>(casting.target_ID, rect.rect);
    return 1;
  }

  int Hit(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    //    Spells::Spell_Hit_Effect(zone, state, caster_ID, position, direction, spellname, targetX, targetY);
    return 1;
  }

  int Cast_Spell(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    Action_Component::Set_State(action, Action_Component::casting);
    float castTime = 500.0f;

    zone.emplace_or_replace<Spells::Cast_Effect>(entity, Cast, Create, Hit);
    zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, "", "Effects02", "");
    return 1;
  }
}// namespace Raise_Zombie
