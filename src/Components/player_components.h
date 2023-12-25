#pragma once
#include "entt/entt.hpp"

namespace Player_Component {

    struct Target_Data {
      Component::Entity_Type entityType;
      entt::entity ID;
      Component::Radius radius;
    };
}
