#pragma once

#include "Maps/World/cave.h"
#include "Movement/movement_functions.h"
#include "Objects/Social/social_control.h"
#include "components.h"
#include "entity_control.h"
#include "loot_panel.h"
#include "mouse_control.h"
#include "player_components.h"
#include "timer.h"
#include "ui.h"

namespace Player_Control {

  void Move_To(entt::registry &zone, entt::entity &entity, Player_Component::Target_Data targetData) {
    zone.remove<Component::Mouse_Move>(entity);
    zone.emplace_or_replace<Component::Moving>(entity);
    zone.emplace_or_replace<Player_Component::Target_Data>(entity, targetData.entityType, targetData.ID, targetData.radius);
  }

  void Update_Keyboard_Movement(entt::registry &zone, entt::entity entity, Component::Velocity &velocity, Component::Input &input, const SDL_Keycode &key) {
    if (zone.any_of<Player_Component::Target_Data, Component::Mouse_Move, Component::Pickup_Item, Component::Attack, Component::Attacking>(entity)) {
      zone.remove<Component::Mouse_Move>(entity);
      zone.remove<Player_Component::Target_Data>(entity);
      zone.remove<Component::Pickup_Item>(entity);
      zone.remove<Component::Attack>(entity);
      zone.remove<Component::Attacking>(entity);
      velocity.magnitude.x = 0.0f;
      velocity.magnitude.y = 0.0f;
    }

    if (velocity.magnitude.x < 0 && input.keyboardControl[key].velocity.x > 0)
      velocity.magnitude.x = 0;
    if (velocity.magnitude.x > 0 && input.keyboardControl[key].velocity.x < 0)
      velocity.magnitude.x = 0;
    if (velocity.magnitude.y > 0 && input.keyboardControl[key].velocity.y < 0)
      velocity.magnitude.y = 0;
    if (velocity.magnitude.y < 0 && input.keyboardControl[key].velocity.y > 0)
      velocity.magnitude.y = 0;

    velocity.magnitude.x += input.keyboardControl[key].velocity.x;
    velocity.magnitude.y += input.keyboardControl[key].velocity.y;

    auto &action = zone.get<Action_Component::Action>(entity);
    zone.emplace_or_replace<Component::Moving>(entity);
    Utilities::Log("Update_Keyboard_Movement() setting state to walk");
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

  void Clear_Moving(entt::registry &zone, entt::entity &entity, Component::Velocity &velocity, Action_Component::Action &action, Action_Component::Action_State actionState) {
    velocity.magnitude.x = 0.0f;
    velocity.magnitude.y = 0.0f;
    Utilities::Log("Clear_Moving()");
    Action_Component::Set_State(action, actionState);
    zone.remove<Player_Component::Target_Data>(entity);
    zone.remove<Component::Moving>(entity);
  }

  bool Attack(entt::registry &zone, entt::entity &entity, Player_Component::Target_Data &targetData, Component::Position &targetPosition, Component::Velocity velocity, Component::Position &position, Component::Melee_Range &meleeRange) {
    if (Entity_Control::Target_In_Range(position, meleeRange.meleeRange, targetPosition, targetData.radius)) {
      if (Social_Control::Check_Relationship(zone, entity, targetData.ID)) {
        Entity_Control::Melee_Attack(zone, entity, targetData.ID, targetPosition);
      } else {
        Social_Control::Greet(zone, entity, targetData.ID);
        //interaction, like shop or whatever
      }
      Utilities::Log("Attack()");
      Clear_Moving(zone, entity, velocity, zone.get<Action_Component::Action>(entity), Action_Component::Action_State::combatIdle);
      zone.emplace_or_replace<Player_Component::Attack_Click_Hold>(entity, targetData, targetPosition, velocity, position, meleeRange);
      return true;
    }
    return false;
  }

  void Click_Attacking_Routine(entt::registry &zone) {
    auto view = zone.view<Player_Component::Attack_Click_Hold, Action_Component::Action>();
    for (auto entity: view) {
      if (!Mouse::bRight_Mouse_Pressed)
        zone.remove<Player_Component::Attack_Click_Hold>(entity);
      auto action = view.get<Action_Component::Action>(entity);
      if (action.frameState == Action_Component::last) {
        auto a = view.get<Player_Component::Attack_Click_Hold>(entity);
        if (zone.get<Component::Alive>(a.targetData.ID).bIsAlive) {
          if (!Attack(zone, entity, a.targetData, a.targetPosition, a.velocity, a.position, a.meleeRange))
            zone.remove<Player_Component::Attack_Click_Hold>(entity);
        } else {
          zone.remove<Player_Component::Attack_Click_Hold>(entity);
        }
      }
    }
  }

  void Interact_With_Object(entt::registry &zone, entt::entity &entity_ID, entt::entity &target_ID, Component::Position &targetPosition) {
    if (!zone.any_of<Component::Loot>(target_ID)) {
      //populate loot drops
      auto &loot = zone.emplace_or_replace<Component::Loot>(target_ID);
      Item_Component::Unit_Equip_Type equip_type = zone.get<Item_Component::Equipment>(entity_ID).type;
      int type = rand() % 3 + 1;
      for (int i = 0; i < type; ++i) {
        Items::Item_Generation item = Items::Generate_Item(zone, equip_type);
        if (item.created) {
          loot.items.emplace_back(item.item);
        }
      }
      Loot_Panel::Set_Loot(loot);
    } else {
      auto &loot = zone.get<Component::Loot>(target_ID);
      //push the drops of the unit into the loot window
      Loot_Panel::Set_Loot(loot);
    }
  }

  void Drop_Item(entt::registry &zone, entt::entity &entity_ID, entt::entity &target_ID, Component::Position &targetPosition) {
    auto &rad = zone.get<Component::Radius>(target_ID);
    Component::Position dropPosition = {targetPosition.x, (targetPosition.y + rad.fRadius + 5.0f)};
    Item_Component::Unit_Equip_Type equip_type = zone.get<Item_Component::Equipment>(entity_ID).type;
    Items::Create_And_Drop_Item(zone, dropPosition, Component::Direction::N, equip_type);
    zone.remove<Component::Radius>(target_ID);
  }

  bool Interact(entt::registry &zone, entt::entity &entity_ID, entt::entity &target_ID, Component::Position &entityPosition, Component::Radius &radius, Component::Position &targetPosition, Component::Radius &targetRadius, Component::Velocity &velocity) {// maybe change to move and attack?
    if (Entity_Control::Target_In_Range(entityPosition, radius.fRadius, targetPosition, targetRadius)) {                                                                                                                                                      //check if center of attack rect is in the target
      auto &action = zone.get<Action_Component::Action>(entity_ID);
      if (action.state != Action_Component::kneel && action.state != Action_Component::struck) {
        Utilities::Log("Interact()");
        Action_Component::Set_State(action, Action_Component::idle);
      }
      Clear_Moving(zone, entity_ID, velocity, action, Action_Component::Action_State::idle);
      Action_Component::Set_State(action, Action_Component::kneel);
      Interact_With_Object(zone, entity_ID, target_ID, targetPosition);
      //      Drop_Item(zone, entity_ID, target_ID, targetPosition);
      return true;
    }
    return false;
    //else move to cursor
  }

  bool Enter_Portal(entt::registry &zone, entt::entity &entity, int &state, Component::Position &position, Component::Position &targetPosition, Component::Velocity &velocity, Player_Component::Target_Data &targetData) {
    if (Entity_Control::Target_In_Range(position, targetData.radius.fRadius, targetPosition, targetData.radius)) {
      Cave::Load_Zone(zone, targetData.ID, state);
      Clear_Moving(zone, entity, velocity, zone.get<Action_Component::Action>(entity), Action_Component::Action_State::idle);
      return true;
    }
    return false;
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

  void Mouse_Move_To_Item(entt::registry &zone, int &state) {//calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
    auto view = zone.view<Component::Position, Component::Velocity, Component::Pickup_Item, Action_Component::Action, Component::Moving>();
    for (auto entity: view) {
      const auto &position = view.get<Component::Position>(entity);
      auto &action = view.get<Action_Component::Action>(entity);
      auto &v = view.get<Component::Velocity>(entity);
      auto &destination = view.get<Component::Pickup_Item>(entity);

      Action_Component::Set_State(action, Action_Component::walk);

      auto moveTo = A_Star::Move_To(zone, entity, position, {destination.x, destination.y});
      Entity_Control::Move_Order(zone, entity, moveTo.x, moveTo.y);

      v.magnitude.x = v.speed * (moveTo.x - position.x);
      v.magnitude.y = v.speed * (moveTo.y - position.y);
      if (Check_If_Arrived(position.x, position.y, destination.x, destination.y)) {
        if (action.state == Action_Component::Action_State::walk) {
          v.magnitude.x = 0.0f;
          v.magnitude.y = 0.0f;
          Action_Component::Set_State(action, Action_Component::Action_State::kneel);
          zone.remove<Component::Moving>(entity);
        }
        //pickup Item
        Equipment_UI::Pick_Up_Item_To_Mouse_Or_Bag(zone, entity, state, destination, Mouse::mouseData.itemCurrentlyHeld);
        zone.remove<Component::Pickup_Item>(entity);
      }
    }
  }

  void Move_To_Target(entt::registry &zone, entt::entity &entity, Component::Position &position, Component::Position &targetPosition, Component::Velocity &velocity, Action_Component::Action &action) {
    if (Movement_Functions::Check_If_Arrived(position, targetPosition)) {
      Clear_Moving(zone, entity, velocity, action, Action_Component::Action_State::idle);
      zone.remove<Player_Component::Target_Data>(entity);
    } else {
      if (action.state != Action_Component::walk) {
        Utilities::Log("Move_To_Target() setting state to walk");
        Action_Component::Set_State(action, Action_Component::walk);
      }

      auto moveTo = A_Star::Move_To(zone, entity, position, {targetPosition.x, targetPosition.y});
      Entity_Control::Move_Order(zone, entity, moveTo.x, moveTo.y);

      velocity.magnitude.x = velocity.speed * (moveTo.x - position.x);
      velocity.magnitude.y = velocity.speed * (moveTo.y - position.y);
    }
  }

  void Mouse_To_Target(entt::registry &zone, int &state) {//calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
    auto view = zone.view<Component::Position, Component::Velocity, Action_Component::Action, Component::Moving, Player_Component::Target_Data, Component::Melee_Range, Component::Radius>();
    for (auto entity: view) {
      //if not in range
      auto &targetData = view.get<Player_Component::Target_Data>(entity);
      if (!zone.any_of<Component::Position>(targetData.ID)) {
        Utilities::Log(std::to_string((int) targetData.ID) + ": has no Position component");
        return;
      }
      auto &position = view.get<Component::Position>(entity);
      auto &radius = view.get<Component::Radius>(entity);
      auto &targetPosition = zone.get<Component::Position>(targetData.ID);
      auto &velocity = view.get<Component::Velocity>(entity);
      //check if the target is in attack range
      auto &meleeRange = view.get<Component::Melee_Range>(entity);
      switch (targetData.entityType) {
        case Component::Entity_Type::object:
          if (Interact(zone, entity, targetData.ID, position, radius, targetPosition, targetData.radius, velocity)) { return; }
          break;
        case Component::Entity_Type::portal:
          if (Enter_Portal(zone, entity, state, position, targetPosition, velocity, targetData)) { return; }
          break;
        case Component::Entity_Type::unit:
          if (Attack(zone, entity, targetData, targetPosition, velocity, position, meleeRange)) { return; }
          break;
        case Component::Entity_Type::foliage:
          break;
        case Component::Entity_Type::item:
          break;
        case Component::Entity_Type::prop:
          break;
        case Component::Entity_Type::building:
          break;
        case Component::Entity_Type::spell:
          break;
        case Component::Entity_Type::tile:
          break;
        case Component::Entity_Type::SIZE:
          break;
      }

      auto &action = view.get<Action_Component::Action>(entity);
      Move_To_Target(zone, entity, position, targetPosition, velocity, action);
    }
  }

  void moveToAttackRoutine(entt::registry &zone, int &state) {
    Click_Attacking_Routine(zone);
    Mouse_Move_To_Item(zone, state);
    Mouse_To_Target(zone, state);
  }
}// namespace Player_Control
