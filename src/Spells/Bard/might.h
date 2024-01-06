#pragma once
#include "mouse_control.h"
#include "spells.h"

namespace Might {

  int Cast(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY) {
//    Spells::Spell_Cast_Effect(zone, state, caster_ID, position, direction, casting.effect, casting.x, casting.y);
    return 1;
  }

  int Create(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY) {
    //create a rect around the caster
    SDL_FRect rect = {0.0f, 0.0f, 1000.0f, 1000.0f};
    SDL_FRect frect = Utilities::Centre_Rect_On_Position(rect, position.x, position.y);
    //get all entities within the rect from the quad tree
    std::vector<entt::entity> entities = Quad_Tree::Get_Nearby_Entities(zone, frect, state);
    //apply the aura to the unit
    for (auto entity : entities) {
      if (!Social_Control::Check_Relationship(zone, caster_ID, entity)) { // if unit is friendly
        auto &entity_position = zone.get<Component::Position>(entity);
        Spells::Spell_Cast_Effect(zone, state, caster_ID, entity_position, direction, casting.effect, entity_position.x, entity_position.y);

        //apply buff
      }
    }
    //repeat periodically like an autocast spell
//    Spells::Create_Spell(zone, state, caster_ID, position, direction, hitEffect, casting, targetX, targetY);
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
      zone.emplace_or_replace<Component::Casting>(entity, castTime, castTime, x, y, "", "001_green", "");
      return 1;
    }
    return 0;
  }
}// namespace Fire