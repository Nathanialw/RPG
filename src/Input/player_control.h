#pragma once

#include "timer.h"
#include "entity_control.h"
#include "components.h"
#include "player_components.h"

namespace Player_Control {
    void Attack_Order(entt::registry &zone, entt::entity &entity, entt::entity &target_ID, Component::Radius &targetRadius) {
        zone.remove<Component::Mouse_Move>(entity);
        zone.emplace_or_replace<Component::Moving>(entity);
        zone.emplace_or_replace<Player_Component::Attack_Move>(entity, target_ID, targetRadius.fRadius);
    }

    float Player_Move_Poll = 0;

    void Mouse_Move_To_Attack(entt::registry &zone) { //calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
        Player_Move_Poll += Timer::timeStep;
        if (Player_Move_Poll >= 0) {
            Player_Move_Poll = 0;
            auto view = zone.view<Component::Position, Component::Velocity, Action_Component::Action, Component::Moving, Player_Component::Attack_Move, Component::Melee_Range>();
            for (auto entity: view) {
                //if not in range
                auto &position = view.get<Component::Position>(entity);
                auto &targetData = view.get<Player_Component::Attack_Move>(entity);
                auto &targetPosition = zone.get<Component::Position>(targetData.ID);
                //check if the target is in attack range
                auto &meleeRange = view.get<Component::Melee_Range>(entity);
                Component::Radius targetRadius;
                targetRadius.fRadius = targetData.hitRadius;
                if (!Entity_Control::Target_In_Melee_Range(zone, position, meleeRange, targetPosition, targetRadius)) {
                    auto &action = view.get<Action_Component::Action>(entity);
                    auto &v = view.get<Component::Velocity>(entity);
                    Action_Component::Set_State(action, Action_Component::walk);

                    v.magnitude.x = v.speed * (targetPosition.x - position.x);
                    v.magnitude.y = v.speed * (targetPosition.y - position.y);
                } else {
                    if (Social_Control::Check_Relationship(zone, entity, targetData.ID)) {
                        Entity_Control::Melee_Attack(zone, entity, targetData.ID, targetPosition);
                    } else {
                        Social_Control::Greet(zone, entity, targetData.ID);
                    }
                    zone.remove<Player_Component::Attack_Move>(entity);
                    zone.remove<Component::Moving>(entity);
                }
            }
        }
    }

    void Mouse_Move_Arrived_Attack_Target(entt::registry &zone) {
        auto view = zone.view<Component::Position, Player_Component::Attack_Move, Component::Moving, Component::Melee_Range>();
        for (auto entity: view) {
            auto &position = view.get<Component::Position>(entity);
            auto &targetData = view.get<Player_Component::Attack_Move>(entity);
            auto &targetPosition = zone.get<Component::Position>(targetData.ID);
            //check if the target is in attack range
            auto &meleeRange = view.get<Component::Melee_Range>(entity);
            Component::Radius targetRadius;
            targetRadius.fRadius = targetData.hitRadius;
            if (Entity_Control::Target_In_Melee_Range(zone, position, meleeRange, targetPosition, targetRadius)) {
                //attack target
                if (Social_Control::Check_Relationship(zone, entity, targetData.ID)) {
                    Entity_Control::Melee_Attack(zone, entity, targetData.ID, targetPosition);
                } else {
                    Social_Control::Greet(zone, entity, targetData.ID);
                }
                zone.remove<Player_Component::Attack_Move>(entity);
                zone.remove<Component::Moving>(entity);
            }
        }
    }

    void Remove_Attack(entt::registry &zone) {
        auto view = zone.view<Player_Component::Attack_Move, Action_Component::Action>();
        for (auto entity: view) {
            auto &action = view.get<Action_Component::Action>(entity);
            if (action.state != Action_Component::walk) {
                zone.remove<Player_Component::Attack_Move>(entity);
                zone.remove<Component::Moving>(entity);
            }
        }
    }

    void Move_To_Atack_Routine(entt::registry &zone) {
        Mouse_Move_To_Attack(zone);
        //		Mouse_Move_Arrived_Attack_Target(zone);
        Remove_Attack(zone);
    }
}
