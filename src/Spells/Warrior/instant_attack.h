#pragma once
#include "Objects/Combat/combat_control.h"
#include "Objects/Social/social_control.h"
#include "action_components.h"
#include "components.h"
#include "damage_text.h"
#include "entt/entt.hpp"
#include "movement_components.h"
#include "utilities.h"

namespace Instant_Attack {

  void Attack(entt::registry &zone, entt::entity &entity, entt::entity &target_ID) {
    auto &action = zone.get<Action_Component::Action>(entity);

    Action_Component::Set_State(action, Action_Component::attack2);
    auto &meleeDamage = zone.get<Component::Melee_Damage>(entity);

    /// calculate damage and show for player
    Component::Damage damageRange = {meleeDamage.minDamage, meleeDamage.maxDamage, meleeDamage.critChance};
    int damage = Combat_Control::Calculate_Damage(damageRange);
    damage *= 2.0f;

    if (zone.any_of<Component::Input>(entity)) {
      // Damage_Text::Add_To_Scrolling_Damage(zone, entity, target_ID, damage, , damageRange.critical);
    }

    auto &struck = zone.get_or_emplace<Component::Struck>(target_ID);

    //                if (meleeDamage.critical) {
    //                    struck.critical = true;
    //                    auto &targetAction = zone.get_or_emplace<Action_Component::Action>(target_ID);
    //                    targetAction.frame = 0;
    //                    targetAction.state = Action_Component::struck;
    //                }
    struck.struck += damage;
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
            auto &velocity = zone.get<Component::Velocity>(player_ID);
            auto &direction = zone.get<Component::Direction>(player_ID);
            direction = Movement_Component::Look_At_Target(position.x, position.y, targetPosition.x, targetPosition.y, velocity.angle, velocity.hexDir);
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

}// namespace Instant_Attack
