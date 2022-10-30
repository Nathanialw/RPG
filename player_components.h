#pragma once
#include "entt.hpp"

namespace Player_Component {
    struct Attack_Move {
        entt::entity ID;
        float x;
        float y;
        float hitRadius;
    };
}
