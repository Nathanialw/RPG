#pragma once

#include "combat_control.h"
#include "components.h"
#include "instant_attack.h"
#include "movement.h"

namespace Sinister_Strike {


  void Attack(entt::registry &zone, entt::entity &entity, entt::entity &target_ID) {
    auto &action = zone.get<Action_Component::Action>(entity);

    Action_Component::Set_State(action, Action_Component::attack2);
    auto &meleeDamage = zone.get<Component::Melee_Damage>(entity);

    Combat_Control::Queue_Hit(zone, entity, target_ID, meleeDamage, Component::skill, 5.0f, Component::Bonus_Damage_Type::add);
  }

  //    have auto attacks "hit" at the start of the animation, the just process them after the swing timer ends
  //    this way you can reset the animation while still having the attack land

  bool Instant_Attack(entt::registry &zone, entt::entity &player_ID) {
    auto view = zone.view<Component::Selected>();
    if (!view.empty()) {
      auto view = zone.view<Component::Selected, Component::Position, Component::Melee_Range, Component::Radius>();
      for (auto target_ID: view) {
        if (Social_Control::Check_Relationship(zone, player_ID, target_ID)) {
          auto &targetPosition = view.get<Component::Position>(target_ID);
          auto &targetRadius = view.get<Component::Radius>(target_ID);
          auto &position = zone.get<Component::Position>(player_ID);
          auto &meleeRange = zone.get<Component::Melee_Range>(player_ID);
          if (Entity_Control::Target_In_Range(position, meleeRange.meleeRange, targetPosition, targetRadius)) {
            auto &angle = zone.get<Component::Velocity>(player_ID).angle;
            auto &direction = zone.get<Component::Direction>(player_ID);
            direction = Movement::Look_At_Target(position.x, position.y, targetPosition.x, targetPosition.y, angle);
            Attack(zone, player_ID, target_ID);
            return true;
          } else {
            //                        say "target is too far away"
            Utilities::Log("target is too far away");
          }
        } else {
          //            say "not a valid, target is not an enemy"
          Utilities::Log("invalid target");
          return false;
        }
      }
    } else {
      //        say "I have no target"
      Utilities::Log("no target");
    }
    return false;
  }

}
