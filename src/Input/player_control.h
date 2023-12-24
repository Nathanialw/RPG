#pragma once

#include "components.h"
#include "entity_control.h"
#include "player_components.h"
#include "timer.h"
#include "ui.h"
#include "social_control.h"
#include "cave.h"

namespace Player_Control {
  void Attack_Order(entt::registry &zone, entt::entity &entity, entt::entity &target_ID, Component::Radius &targetRadius) {
    zone.remove<Component::Mouse_Move>(entity);
    zone.emplace_or_replace<Component::Moving>(entity);
    zone.emplace_or_replace<Player_Component::Attack_Move>(entity, target_ID, targetRadius.fRadius);
  }

  void Interact_Order(entt::registry &zone, entt::entity &entity, entt::entity &target_ID, Component::Radius &targetRadius) {
    zone.remove<Component::Mouse_Move>(entity);
    zone.emplace_or_replace<Component::Moving>(entity);
    zone.emplace_or_replace<Player_Component::Interact_Move>(entity, target_ID, targetRadius.fRadius);
  }

  void Portal_Order(entt::registry &zone, entt::entity &entity, entt::entity &target_ID, Component::Radius &targetRadius) {
    zone.remove<Component::Mouse_Move>(entity);
    zone.emplace_or_replace<Component::Moving>(entity);
    zone.emplace_or_replace<Player_Component::Interact_Portal>(entity, target_ID, targetRadius.fRadius);
  }


  void Update_Keyboard_Movement(entt::registry &zone, entt::entity entity, Component::Velocity &velocity, Component::Input &input, const SDL_Keycode &key) {
    if (zone.any_of<Player_Component::Interact_Move, Player_Component::Attack_Move, Component::Mouse_Move, Component::Pickup_Item>(entity)) {
      zone.remove<Player_Component::Interact_Move>(entity);
      zone.remove<Player_Component::Attack_Move>(entity);
      zone.remove<Component::Mouse_Move>(entity);
      zone.remove<Component::Pickup_Item>(entity);
      velocity.magnitude.x = 0.0f;
      velocity.magnitude.y = 0.0f;
    }

    if (velocity.magnitude.x < 0 && input.keyboardControl[key].velocity.x > 0) {
      velocity.magnitude.x = 0;
    }
    if (velocity.magnitude.x > 0 && input.keyboardControl[key].velocity.x < 0) {
      velocity.magnitude.x = 0;
    }
    if (velocity.magnitude.y > 0 && input.keyboardControl[key].velocity.y < 0) {
      velocity.magnitude.y = 0;
    }
    if (velocity.magnitude.y < 0 && input.keyboardControl[key].velocity.y > 0) {
      velocity.magnitude.y = 0;
    }
    velocity.magnitude.x += input.keyboardControl[key].velocity.x;
    velocity.magnitude.y += input.keyboardControl[key].velocity.y;

    auto &action = zone.get<Action_Component::Action>(entity);
    zone.emplace_or_replace<Component::Moving>(entity);
    Action_Component::Set_State(action, Action_Component::walk);
  }

  void Check_Pressed_Keys(entt::registry &zone, entt::entity entity) {
    if (zone.any_of<Component::Input>(entity) && zone.any_of<Component::Velocity>(entity)) {
      auto &input = zone.get<Component::Input>(entity);
      auto &velocity = zone.get<Component::Velocity>(entity);

      for (auto const &[key, value]: input.keyboardControl) {
        if (value.pressed) {
          Update_Keyboard_Movement(zone, entity, velocity, input, key);
        }
      }
    }
  }

  float Player_Move_Poll = 0;

  void Mouse_Move_To_Attack(entt::registry &zone) {//calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
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
        if (!Entity_Control::Target_In_Range(position, meleeRange.meleeRange, targetPosition, targetRadius)) {
          auto &action = view.get<Action_Component::Action>(entity);
          auto &v = view.get<Component::Velocity>(entity);
          Action_Component::Set_State(action, Action_Component::walk);

          v.magnitude.x = v.speed * (targetPosition.x - position.x);
          v.magnitude.y = v.speed * (targetPosition.y - position.y);
        } else {
          if (Social_Control::Check_Relationship(zone, entity, targetData.ID)) {
            Entity_Control::Melee_Attack(zone, entity, targetData.ID, targetPosition);
            auto &v = view.get<Component::Velocity>(entity);
            v.magnitude.x = 0.0f;
            v.magnitude.y = 0.0f;
          } else {
            Social_Control::Greet(zone, entity, targetData.ID);
            auto &v = view.get<Component::Velocity>(entity);
            v.magnitude.x = 0.0f;
            v.magnitude.y = 0.0f;
          }
          zone.remove<Player_Component::Attack_Move>(entity);
          zone.remove<Component::Moving>(entity);
        }
      }
    }
  }

  void Interact_With_Object() {
    Utilities::Log("interacting with object");
  }

  void Interact(entt::registry &zone, entt::entity &entity_ID, entt::entity &target_ID, Component::Position &entityPosition, float &radius, Component::Position &targetPosition, Component::Radius &targetRadius) {// maybe change to move and attack?
    //calcuate the point to move to that puts in range of melee attack on every few frames
    //if it is in range, run Melee_Attack()
    //else pass that point as an update to the move order
    if (Entity_Control::Target_In_Range(entityPosition, radius, targetPosition, targetRadius)) {//check if center of attack rect is in the target
      auto &action = zone.get<Action_Component::Action>(entity_ID);
      if (action.state != Action_Component::kneel && action.state != Action_Component::struck) {
        Action_Component::Set_State(action, Action_Component::idle);
      }
      Action_Component::Set_State(action, Action_Component::kneel);
      Interact_With_Object();
      zone.remove<Component::Mouse_Move>(entity_ID);
      zone.remove<Component::Moving>(entity_ID);
    } else {
      Entity_Control::Move_Order(zone, entity_ID, targetPosition.x, targetPosition.y);
    }
    //else move to cursor
  }

  void Mouse_Move_To_Interact(entt::registry &zone) {//calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
    if (Player_Move_Poll >= 0) {
      Player_Move_Poll = 0;
      auto view = zone.view<Component::Position, Component::Velocity, Action_Component::Action, Component::Moving, Player_Component::Interact_Move, Component::Radius>();
      for (auto entity: view) {
        //if not in range
        auto &position = view.get<Component::Position>(entity);
        auto &targetData = view.get<Player_Component::Interact_Move>(entity);
        auto &targetPosition = zone.get<Component::Position>(targetData.ID);
        //check if the target is in attack range
        auto &radius = view.get<Component::Radius>(entity);
        Component::Radius targetRadius;
        targetRadius.fRadius = targetData.hitRadius;
        auto &action = view.get<Action_Component::Action>(entity);
        if (!Entity_Control::Target_In_Range(position, radius.fRadius, targetPosition, targetRadius)) {
          auto &v = view.get<Component::Velocity>(entity);
          Action_Component::Set_State(action, Action_Component::walk);
          v.magnitude.x = v.speed * (targetPosition.x - position.x);
          v.magnitude.y = v.speed * (targetPosition.y - position.y);
        } else {
          Interact(zone, entity, targetData.ID, position, radius.fRadius, targetPosition, targetRadius);
          auto &v = view.get<Component::Velocity>(entity);
          v.magnitude.x = 0.0f;
          v.magnitude.y = 0.0f;
          zone.remove<Player_Component::Interact_Move>(entity);
          zone.remove<Component::Moving>(entity);
        }
      }
    }
  }


  void Enter_Portal (entt::registry &zone, World::GameState &state, entt::entity &portalID) {
    if (state == World::GameState::cave) {
      Cave::Exit_Cave();
    }
    else {
      Cave::Enter_Cave();
    }
  }

  void Mouse_Move_To_Portal(entt::registry &zone, World::GameState &state) {//calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
    if (Player_Move_Poll >= 0) {
      Player_Move_Poll = 0;
      auto view = zone.view<Component::Position, Component::Velocity, Action_Component::Action, Component::Moving, Player_Component::Interact_Portal, Component::Radius>();
      for (auto entity: view) {
        //if not in range
        auto &position = view.get<Component::Position>(entity);
        auto &targetData = view.get<Player_Component::Interact_Portal>(entity);
        auto &targetPosition = zone.get<Component::Position>(targetData.ID);
        //check if the target is in attack range
        auto &radius = view.get<Component::Radius>(entity);
        Component::Radius targetRadius;
        targetRadius.fRadius = targetData.hitRadius;
        auto &action = view.get<Action_Component::Action>(entity);
        if (!Entity_Control::Target_In_Range(position, radius.fRadius, targetPosition, targetRadius)) {
          auto &v = view.get<Component::Velocity>(entity);
          Action_Component::Set_State(action, Action_Component::walk);
          v.magnitude.x = v.speed * (targetPosition.x - position.x);
          v.magnitude.y = v.speed * (targetPosition.y - position.y);
        } else {
          Enter_Portal(zone, state, targetData.ID);
          auto &v = view.get<Component::Velocity>(entity);
          v.magnitude.x = 0.0f;
          v.magnitude.y = 0.0f;
          zone.remove<Player_Component::Interact_Move>(entity);
          zone.remove<Component::Moving>(entity);
        }
      }
    }
  }

  bool Check_If_Arrived(const float &unitX, const float &unitY, const float &destinationX, const float &destinationY) {
    float accuracy = 5.0f;
    if (unitX + accuracy > destinationX &&
        unitX - accuracy < destinationX &&
        unitY - accuracy < destinationY &&
        unitY + accuracy > destinationY) {
      return true;
    } else {
      return false;
    }
  }

  void Mouse_Move_To_Item(entt::registry &zone, World::GameState &state) {//calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
    if (Player_Move_Poll >= 0) {
      Player_Move_Poll = 0;
      auto view = zone.view<Component::Position, Component::Velocity, Component::Pickup_Item, Action_Component::Action, Component::Moving>();
      for (auto entity: view) {
        const auto &x = view.get<Component::Position>(entity);
        const auto &y = view.get<Component::Position>(entity);
        auto &action = view.get<Action_Component::Action>(entity);
        auto &v = view.get<Component::Velocity>(entity);
        auto &mov = view.get<Component::Pickup_Item>(entity);
        Action_Component::Set_State(action, Action_Component::walk);
        v.magnitude.x = v.speed * (mov.x - x.x);
        v.magnitude.y = v.speed * (mov.y - y.y);
        if (Check_If_Arrived(x.x, y.y, mov.x, mov.y)) {
          if (action.state == Action_Component::Action_State::walk) {
            v.magnitude.x = 0.0f;
            v.magnitude.y = 0.0f;
            Action_Component::Set_State(action, Action_Component::Action_State::kneel);
            zone.remove<Component::Moving>(entity);
          }
          //pickup Item
          UI::Pick_Up_Item_To_Mouse_Or_Bag(zone, entity, state, mov, Mouse::itemCurrentlyHeld);
          zone.remove<Component::Pickup_Item>(entity);
        }
      }
    }
  }

  //    void Mouse_Move_Arrived_Pickup_Item(entt::registry & zone, bool &isItemCurrentlyHeld) {
  //      auto view = zone.view<Component::Position, Component::Velocity, Action_Component::Action, Component::Pickup_Item>();
  //      for (auto entity: view) {
  //        auto &action = view.get<Action_Component::Action>(entity);
  //        auto &v = view.get<Component::Velocity>(entity);
  //        const auto &position = view.get<Component::Position>(entity);
  //        auto &itemData = view.get<Component::Pickup_Item>(entity);
  //        if (Check_If_Arrived(position.x, position.y, itemData.x, itemData.y)) {
  //          if (action.state == Action_Component::Action_State::walk) {
  //            v.magnitude.x = 0.0f;
  //            v.magnitude.y = 0.0f;
  //            Action_Component::Set_State(action, Action_Component::Action_State::idle);
  //            zone.remove<Component::Moving>(entity);
  //          }
  //
  //          //pickup Item
  //          UI::Pick_Up_Item_To_Mouse_Or_Bag(zone, itemData, isItemCurrentlyHeld);
  //          zone.remove<Component::Pickup_Item>(entity);
  //        }
  //        //      if (action.state == Action_Component::Action_State::idle) {
  //        //        zone.remove<Component::Moving>(entity);
  //        //        zone.remove<Component::Pickup_Item>(entity);
  //        //      }
  //      }

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
      if (Entity_Control::Target_In_Range(position, meleeRange.meleeRange, targetPosition, targetRadius)) {
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


//  void Move_To_Item_Routine(entt::registry &zone, bool isItemCurrentlyHeld) {
//    Mouse_Move_To_Item();
//    //    Mouse_Move_Arrived_Pickup_Item(zone, isItemCurrentlyHeld);
//  }

  void Move_To_Atack_Routine(entt::registry &zone, World::GameState &state) {
    Player_Move_Poll += Timer::timeStep;
    Mouse_Move_To_Interact(zone);
    Mouse_Move_To_Attack(zone);
    Mouse_Move_To_Item(zone, state);
    Mouse_Move_To_Portal(zone, state);
    //		Mouse_Move_Arrived_Attack_Target(zone);
    Remove_Attack(zone);
  }
}// namespace Player_Control
