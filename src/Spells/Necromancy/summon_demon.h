#pragma once
#include "components.h"
#include "entt/entt.hpp"
#include "spells.h"

namespace Summon_Demon {

  int Casting(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
    Component::Position targetPosition = {Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse};
    Spells::Spell_Cast_Effect(zone, state, caster_ID, targetPosition, direction, casting.effect, targetX, targetY);
    return 1;
  }

  int Create(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
    db::Unit_Data data = Game_Objects_Lists::units["demons"][0];
    Component::Unit_Index unitIndex = {"demons", 0};

    Social_Component::Summon summon;
    summon.relationships = zone.get<Social_Component::Relationships>(caster_ID);
    summon.summon = true;
    summon.blendType = Social_Component::normal;
    summon.race = zone.get<Social_Component::Race>(caster_ID);
    Component::Position targetPosition = {casting.x, casting.y};
    Create_Entities::Create_Entity(zone, state, casting.x, casting.y, data, false, summon, unitIndex);
    return 1;
  }

  int Hit(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY) {
    //    Spells::Spell_Hit_Effect(zone, state, caster_ID, position, direction, spellname, targetX, targetY);
    return 1;
  }

  int Cast(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Action_Component::Set_State(action, Action_Component::casting);
    float castTime = 500.0f;

    zone.emplace_or_replace<Spells::Cast_Effect>(entity, Casting, Create, Hit);
    zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, "", "Effects02", "");
    return 1;
  }
}// namespace Summon_Demon