//
// Created by desktop on 12/4/24.
//
#pragma once

#include "components.h"
#include "mouse_control.h"
#include "player_components.h"
#include "player_control.h"
#include "entt/entt.hpp"

namespace Warrior {

    void Toggle_Auto_Attack(entt::registry &zone) {
        auto view = zone.view<Component::Input, Component::Melee_Range, Component::Position, Component::Velocity>();
        for (auto entity: view) {
            if (zone.any_of<Player_Component::Auto_Attack>(entity))
                zone.remove<Player_Component::Auto_Attack>(entity);
            else {
                auto target_view = zone.view<Component::Selected, Component::Position, Component::Velocity, Component::Melee_Range, Component::Alive, Component::Entity_Type, Component::Radius>();
                for (auto target_entity: target_view) {
                    if (target_view.get<Component::Alive>(target_entity).bIsAlive) {
                        zone.emplace_or_replace<Player_Component::Auto_Attack>(entity);
                    }
                }
            }
        }
    }

    int Auto_Attack(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
        Toggle_Auto_Attack(zone);
        return 0;
    }

}