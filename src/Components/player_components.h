#pragma once

#include "entt/entt.hpp"

namespace Player_Component {

    struct Target_Data {
        Component::Entity_Type entityType;
        entt::entity ID;
        Component::Radius radius;
    };

    struct Attack_Click_Hold {
        Player_Component::Target_Data targetData;
        Component::Position targetPosition;
        Component::Velocity velocity;
        Component::Position position;
        Component::Melee_Range meleeRange;
    };

    struct Auto_Attack {
    };
}// namespace Player_Component
