#pragma once
#include "action_components.h"
#include "components.h"
#include "entt/entt.hpp"


namespace Spells {
  typedef int (*spells)(entt::registry &zone, entt::entity &entity, float &x, float &y, const char *name);

  typedef int (*castSpell)(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y);
  typedef int (*Hit)(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY);

  int Hit_Template(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, const char *spellname, float &targetX, float &targetY){
    return 1;
  }

  struct Hit_Effect {
    Hit hit = Hit_Template;
  };

  typedef int (*Cast)(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Spells::Hit &hitEffect, Component::Casting &casting, float &targetX, float &targetY);
  typedef int (*castEffect)(entt::registry &zone, int &state, entt::entity &caster_ID, Component::Position &position, Component::Direction &direction, Component::Casting &casting, float &targetX, float &targetY);

}// namespace Spells