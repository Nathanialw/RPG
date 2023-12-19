#pragma once

#include "action_components.h"
#include "timer.h"
#include "entity_control.h"
#include "components.h"
#include "utilities.h"
#include "social_control.h"
#include "mouse_control.h"
#include "world.h"

namespace Movement {

  namespace {
    float Player_Move_Poll = 0;
    float Update_Position_Poll = 0;
    float linearMovePoll = 0;
  }

  void Mouse_Moving(entt::registry &zone) { // maybe change to move and attack?
    auto view = zone.view<Component::Input>();
    for (auto player_ID: view) {
      if (Mouse::bRight_Mouse_Pressed) {
        if (Social_Control::Enemy_Selected(zone, player_ID)) {
          Entity_Control::Move_Order(zone, player_ID, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
        }
      }
    }
  }

  const float timeStep = 1.0f / 60.0f;

  void Update_Position() {
    auto view = World::zone.view<Action_Component::Action, Component::Position, Component::Velocity, Component::Moving, Component::Body>();
    Update_Position_Poll += Timer::timeStep;
    float angleY = 0.0f;
    //std::cout << Update_Position_Poll << std::endl;
    if (Update_Position_Poll >= 0) {
      for (auto entity: view) {
        auto &velocity = view.get<Component::Velocity>(entity);
        auto &position = view.get<Component::Position>(entity);
        auto &action = view.get<Action_Component::Action>(entity);

        if (action.state == Action_Component::attack) {
          velocity.magnitude.x = 0.0f;
          velocity.magnitude.y = 0.0f;
        }

        if (velocity.magnitude.x != 0 || velocity.magnitude.y != 0) {
          if (fabs(velocity.magnitude.x) < 0.1) { velocity.magnitude.x = 0; }; //clamp rounding errors
          if (fabs(velocity.magnitude.y) < 0.1) { velocity.magnitude.y = 0; };
          velocity.angle = atan2f(velocity.magnitude.x, velocity.magnitude.y);
          angleY = atan2f(velocity.magnitude.y, velocity.magnitude.x);
          float velocityX = sinf(velocity.angle) * velocity.speed;
          float velocityY = sinf(angleY) * velocity.speed;
          auto &pBody = view.get<Component::Body>(entity).body;

          velocity.dX = velocityX * (float) Update_Position_Poll * 5000000.0f;
          velocity.dY = velocityY * (float) Update_Position_Poll * 5000000.0f;

          b2Vec2 impulse = {velocity.dX, velocity.dY};
          pBody->ApplyForce(impulse, pBody->GetWorldCenter(), true);
        }

        if ((velocity.magnitude.x == 0.0f) && (velocity.magnitude.y == 0.0f)) {
          World::zone.remove<Component::Moving>(entity);
          Action_Component::Set_State(action, Action_Component::idle);
        }
      }
      Update_Position_Poll = 0;
    }
  };

  Component::Direction Set_Direction(float angleInRadians) {
    if (angleInRadians > 2.74889 || angleInRadians <= (-2.74889)) { return Component::Direction::N; }
    else if (angleInRadians > 1.96349 && angleInRadians <= (2.74889)) { return Component::Direction::NE; }
    else if (angleInRadians > 1.17809 && angleInRadians <= (1.96349)) { return Component::Direction::E; }
    else if (angleInRadians > 0.39269 && angleInRadians <= (1.17809)) { return Component::Direction::SE; }
    else if (angleInRadians > (-0.39269) && angleInRadians <= (0.39269)) { return Component::Direction::S; }
    else if (angleInRadians > (-1.17811) && angleInRadians <= (-0.39269)) { return Component::Direction::SW; }
    else if (angleInRadians > (-1.96351) && angleInRadians <= (-1.17811)) { return Component::Direction::W; }
    else if (angleInRadians > (-2.74889) && angleInRadians <= (-1.96351)) { return Component::Direction::NW; }

    Utilities::Log("Set_Direction() fallthrough error");
    return Component::Direction::N;
  }

  Component::Direction Look_At_Target(float &positionX, float &positionY, float &targetX, float &targetY, float &angleInRadians) {
    angleInRadians = Utilities::Get_Direction_Point(positionX, positionY, targetX, targetY);
    return Set_Direction(angleInRadians);
  }

  void Update_Direction() {
    auto view = World::zone.view<Component::Direction, Action_Component::Action, Component::Velocity, Component::Moving>();
    for (auto entity: view) {
      auto &vel = view.get<Component::Velocity>(entity);
      auto &direction = view.get<Component::Direction>(entity);
      auto &action = view.get<Action_Component::Action>(entity);

      direction = Set_Direction(vel.angle);

      if (action.state == Action_Component::walk) {
        if (vel.magnitude.x == 0 && vel.magnitude.y == 0) {
          Action_Component::Set_State(action, Action_Component::idle);
        };
      }
    }
  }

  //takes in the x,y of the unit and checks if it is within the destination x,y within an accuracy of the set variable
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

  void Mouse_Move_Arrived() {
    auto view = World::zone.view<Rendering_Components::Sprite_Sheet_Info, Component::Position, Component::Velocity, Action_Component::Action, Component::Mouse_Move, Component::Body>();
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

          World::zone.remove<Component::Mouse_Move>(entity);
          World::zone.remove<Component::Moving>(entity);
          Player_Control::Check_Pressed_Keys(World::zone, entity);
        }
      }
    }
  }

  void Mouse_Move_To() { //calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
    Player_Move_Poll += Timer::timeStep;
    if (Player_Move_Poll >= 0) {
      Player_Move_Poll = 0;
      auto view = World::zone.view<Component::Position, Component::Velocity, Component::Mouse_Move, Action_Component::Action, Component::Moving, Component::Body>();
      for (auto entity: view) {
        const auto &position = view.get<Component::Position>(entity);
        auto &action = view.get<Action_Component::Action>(entity);
        auto &v = view.get<Component::Velocity>(entity);
        auto &mov = view.get<Component::Mouse_Move>(entity);

        if (action.state == Action_Component::idle) {
          Action_Component::Set_State(action, Action_Component::walk);
        }
        World::zone.remove<Player_Component::Interact_Move>(entity);
        World::zone.remove<Player_Component::Attack_Move>(entity);
        World::zone.remove<Component::Pickup_Item>(entity);

        v.magnitude.x = v.speed * (mov.fX_Destination - position.x);
        v.magnitude.y = v.speed * (mov.fY_Destination - position.y);
      }
    }
  }

  void Linear_Move_To() {
    linearMovePoll += Timer::timeStep;
    if (linearMovePoll >= 50) {
      auto view = World::zone.view<Component::Velocity, Action_Component::Action, Component::Moving, Component::Linear_Move, Component::Spell_Range>();
      for (auto entity: view) {
        auto &action = view.get<Action_Component::Action>(entity);
        auto &v = view.get<Component::Velocity>(entity);
        auto &mov = view.get<Component::Linear_Move>(entity);
        auto &range = view.get<Component::Spell_Range>(entity);

        if (range.fRange <= 1500) {
          Action_Component::Set_State(action, Action_Component::walk);
          v.magnitude.x = v.speed * (mov.fX_Direction - range.fSourceX);
          v.magnitude.y = v.speed * (mov.fY_Direction - range.fSourceY);
          range.fRange += linearMovePoll;
        } else {
          World::zone.remove<Component::Linear_Move>(entity);
        }
      }
      linearMovePoll = 0;
    }
  }

  void Update_Entity_Positions(entt::registry &zone) {
    //Mouse_Attack_Move(); //runs every frame to see if mouse is down, if it is it moves you to the new location
    Mouse_Moving(zone);
    Linear_Move_To();
    Mouse_Move_To();
    Mouse_Move_Arrived();
    Update_Direction();
    Update_Position();
  }
}
