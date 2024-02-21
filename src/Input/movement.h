#pragma once

#include "Movement/movement_functions.h"
#include "action_components.h"
#include "components.h"
#include "entity_control.h"
#include "mouse_control.h"
#include "movement_components.h"
#include "social_control.h"
#include "timer.h"
#include "utilities.h"
#include "world.h"

namespace Movement {

  namespace {
    float Player_Move_Poll = 0.0f;
    float Update_Position_Poll = 0.0f;
    float linearMovePoll = 0.0f;
  }// namespace

  using namespace Movement_Functions;

  void Mouse_Moving(entt::registry &zone) {// maybe change to move and attack?
    auto view = zone.view<Component::Input>();
    for (auto player_ID: view) {
      if (Mouse::bRight_Mouse_Pressed) {
        if (SDL_GetMouseState(NULL, NULL) != SDL_BUTTON(SDL_BUTTON_RIGHT) && !Hotbar::autoRun) {
          Mouse::bRight_Mouse_Pressed = false;
        }
        //if I have an ally selected I don't want to move when I click the button, but I DO want to still use autorun
        else if (Social_Control::Enemy_Selected(zone, player_ID)) {
          Entity_Control::Move_Order(zone, player_ID, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
        }
      }
    }
  }

  void Update_Position(entt::registry &zone) {
    auto view = zone.view<Action_Component::Action, Component::Velocity, Component::Moving, Component::Body>();
    float angleY = 0.0f;
    for (auto entity: view) {
      auto &velocity = view.get<Component::Velocity>(entity);
      auto &action = view.get<Action_Component::Action>(entity);

      if (zone.any_of<Component::Input>(entity)) {
        Loot_Panel::Close();
      }

      if (action.state == Action_Component::attack) {
        velocity.magnitude.x = 0.0f;
        velocity.magnitude.y = 0.0f;
      }

      if (velocity.magnitude.x != 0 || velocity.magnitude.y != 0) {
        if (fabs(velocity.magnitude.x) < 0.1) { velocity.magnitude.x = 0; };//clamp rounding errors
        if (fabs(velocity.magnitude.y) < 0.1) { velocity.magnitude.y = 0; };
        velocity.angle = atan2f(velocity.magnitude.x, velocity.magnitude.y);
        angleY = atan2f(velocity.magnitude.y, velocity.magnitude.x);
        float velocityX = sinf(velocity.angle) * velocity.speed;
        float velocityY = sinf(angleY) * velocity.speed;
        auto &pBody = view.get<Component::Body>(entity).body;

        velocity.dY = velocityY * (float) Timer::timeStep * 5000000.0f;
        velocity.dX = velocityX * (float) Timer::timeStep * 5000000.0f;

        b2Vec2 impulse = {velocity.dX, velocity.dY};
        pBody->ApplyForce(impulse, pBody->GetWorldCenter(), true);
      }

      if ((velocity.magnitude.x == 0.0f) && (velocity.magnitude.y == 0.0f)) {
        zone.remove<Component::Moving>(entity);
        Utilities::Log("Update_Position()");
        Action_Component::Set_State(action, Action_Component::idle);
      }
    }
  };


  void Update_Direction(entt::registry &zone) {
    auto view = zone.view<Component::Direction, Action_Component::Action, Component::Velocity, Component::Moving>();
    for (auto entity: view) {
      auto &vel = view.get<Component::Velocity>(entity);
      auto &direction = view.get<Component::Direction>(entity);
      auto &action = view.get<Action_Component::Action>(entity);

      direction = Movement_Component::Set_Direction(vel.angle, vel.hexDir);

      if (action.state == Action_Component::walk) {
        if (vel.magnitude.x == 0 && vel.magnitude.y == 0) {
          Utilities::Log("Update_Direction()");
          Action_Component::Set_State(action, Action_Component::idle);
        };
      }
    }
  }


  void Mouse_Move_Arrived(entt::registry &zone) {
    auto view = zone.view<Rendering_Components::Sprite_Sheet_Info, Component::Position, Component::Velocity, Action_Component::Action, Component::Mouse_Move, Component::Body>();
    for (auto entity: view) {
      auto &v = view.get<Component::Velocity>(entity);
      const auto &x = view.get<Component::Position>(entity);
      const auto &y = view.get<Component::Position>(entity);
      const auto &mov = view.get<Component::Mouse_Move>(entity);
      auto &action = view.get<Action_Component::Action>(entity);

      if (Check_If_Arrived(x.x, y.y, mov.fX_Destination, mov.fY_Destination)) {
        if (action.state == Action_Component::walk) {
          v.magnitude.x = 0.0f;
          v.magnitude.y = 0.0f;
          Action_Component::Set_State(action, Action_Component::Action_State::idle);

          zone.remove<Component::Mouse_Move>(entity);
          zone.remove<Component::Moving>(entity);
          Player_Control::Check_Pressed_Keys(zone, entity);
        }
      }
    }
  }

  void Mouse_Move_To(entt::registry &zone) {//calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
    Player_Move_Poll += Timer::timeStep;
    if (Player_Move_Poll >= 0) {
      Player_Move_Poll = 0;
      auto view = zone.view<Component::Position, Component::Velocity, Component::Mouse_Move, Action_Component::Action, Component::Moving, Component::Body>(entt::exclude<Player_Component::Target_Data>);
      for (auto entity: view) {
        const auto &position = view.get<Component::Position>(entity);
        auto &action = view.get<Action_Component::Action>(entity);
        auto &v = view.get<Component::Velocity>(entity);
        auto &mov = view.get<Component::Mouse_Move>(entity);

        if (action.state == Action_Component::idle || action.state == Action_Component::combatIdle) {
          Utilities::Log("Mouse_Move_To() setting state to walk");
          Action_Component::Set_State(action, Action_Component::walk);
        }
        zone.remove<Component::Pickup_Item>(entity);

        v.magnitude.x = v.speed * (mov.fX_Destination - position.x);
        v.magnitude.y = v.speed * (mov.fY_Destination - position.y);
      }
    }
  }

  void Linear_Move_To(entt::registry &zone) {
    auto view = zone.view<Component::Velocity, Action_Component::Action, Component::Moving, Component::Linear_Move, Component::Spell_Range>();
    for (auto entity: view) {
      auto &action = view.get<Action_Component::Action>(entity);
      auto &v = view.get<Component::Velocity>(entity);
      auto &mov = view.get<Component::Linear_Move>(entity);
      auto &range = view.get<Component::Spell_Range>(entity);

      if (range.fRange <= 0) {
        zone.remove<Component::Linear_Move>(entity);
      } else {
        //        Utilities::Log("Linear_Move_To() setting state to walk");
        Action_Component::Set_State(action, Action_Component::walk);
        v.magnitude.x = v.speed * (mov.fX_Direction - range.fSourceX);
        v.magnitude.y = v.speed * (mov.fY_Direction - range.fSourceY);
        range.fRange -= v.speed * Timer::timeStep;
      }
    }
  }

  void Update_Entity_Positions(entt::registry &zone) {
    //Mouse_Attack_Move(); //runs every frame to see if mouse is down, if it is it moves you to the new location
    Mouse_Moving(zone);
    Linear_Move_To(zone);
    Mouse_Move_To(zone);
    Mouse_Move_Arrived(zone);
    Update_Direction(zone);
    Update_Position(zone);
  }
}// namespace Movement
