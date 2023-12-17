#pragma once
#include "entt/entt.hpp"

namespace Player_Component {
    struct Attack_Move {
        entt::entity ID;
        float hitRadius;
    };

    struct Interact_Move {
        entt::entity ID;
        float hitRadius;
    };
}
