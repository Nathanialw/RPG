#pragma once
#include "action_components.h"
#include "entt/entt.hpp"
#include "components.h"

namespace Entity_Control {
    void Move_Order(entt::registry& zone, entt::entity& entity, float& x, float& y) {
        zone.emplace_or_replace<Component::Mouse_Move>(entity, x, y);
        zone.emplace_or_replace<Component::Moving>(entity);
    }

//    void Attack_Move_Order(entt::registry& zone, entt::entity& entity, float& x, float& y, entt::entity &target) {
//        auto targetPosition = zone.get<Position>(target);
//        zone.emplace_or_replace<Component::Mouse_Move>(entity, targetPosition.x, targetPosition.y);
//        zone.emplace_or_replace<Component::Moving>(entity);
//    }
//
//    void Update_Attack_Move_Order(entt::registry& zone, entt::entity& entity, float& x, float& y, entt::entity &target) {
//        //grab everyone "attack moving"
//        auto view = zone.view<AI_Components::Attack_Moving>();
//        for (auto entity : view) {
//            auto targetPosition = zone.get<Position>(target);
//            zone.emplace_or_replace<Component::Mouse_Move>(entity, targetPosition.x, targetPosition.y);
//            zone.emplace_or_replace<Component::Moving>(entity);
//        }
//    }

    void Spell_Attack(entt::registry& zone, entt::entity& entity, float& x, float& y, const char* name) {
        if (zone.any_of<Component::Casting>(entity) == false) { //locks out casting until cast animation has finished
            auto &action = zone.get<Action_Component::Action>(entity);
            Action_Component::Set_State(action, Action_Component::casting);
            zone.remove<Component::Moving>(entity);
            zone.emplace_or_replace<Component::Casting>(entity, 2000, 2000, x, y);
            zone.emplace_or_replace<Component::Spell_Name>(entity, name);
        }
    }

    void Melee_Attack(entt::registry& zone, entt::entity& entity_ID, entt::entity& target_ID, Component::Position &targetPosition) {
        if (zone.any_of<Component::Attacking>(entity_ID) == false) { //locks out attacking until attack animation has finished
            zone.emplace_or_replace<Component::Attack>(entity_ID, target_ID, targetPosition.x, targetPosition.y);
        }
    }

    bool Target_In_Melee_Range(entt::registry& zone, Component::Position &entityPosition, Component::Melee_Range &meleeRange, Component::Position &targetPosition, Component::Radius &targetRadius) {
        float length = entityPosition.x - targetPosition.x;
        float width = entityPosition.y - targetPosition.y;

        if (((length * length) + (width * width)) <= ((meleeRange.meleeRange + targetRadius.fRadius) * (meleeRange.meleeRange + targetRadius.fRadius))) {
            //float range = Utilities::Get_Hypotenuse(length, width);
            //if (range <= (meleeRange.meleeRange + targetRadius.fRadius)) {
            return true;
            //}
        }
        return false;
    }
}
