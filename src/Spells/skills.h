#pragma once
#include "Maps/World/world.h"
#include "action_components.h"
#include "entt/entt.hpp"

namespace Skills {

  void Feign_Death(entt::registry &zone, entt::entity &unit) {

    //need to set position of frame properly
    //        zone.get<Action_Component::Action>(unit).state = Action_Component::dying;
    //        zone.get<Rendering_Components::Sprite_Sheet_Info>(unit).frameIndex = 0;
  }
}// namespace Skills