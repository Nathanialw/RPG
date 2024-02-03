#pragma once
#include "Buildings/building_functions.h"
#include "collision_components.h"
#include "components.h"
#include "entt/entt.hpp"
#include "timer.h"
#include "tmxlite/Object.hpp"
#include "world.h"

namespace Collision {
  //  std::map<World::GameState, b2World*> world;
  std::vector<b2World *> collisionList(World::numZones);

  const float timeStep = 1.0f / 5.0f;
  const float M2P = 60.0f;
  const float P2M = 1.0f / M2P;
  int32 velocityIterations = 6;
  int32 positionIterations = 8;

  float collision_Timestep = 0.0f;

  class MyContactListener : public b2ContactListener {
    void BeginContact(b2Contact *contact) override {
      Building_Functions::Show_Interior(contact);
    }

    void EndContact(b2Contact *contact) override {
      Building_Functions::Show_Exterior(contact);
    }
  };
  MyContactListener myContactListenerInstance;

  void init_Collison(const int &state) {
    b2Vec2 gravity{};
    gravity.SetZero();
    collisionList.at(state) = new b2World(gravity);
    collisionList[state]->SetAutoClearForces(false);
    collisionList[state]->SetContactListener(&myContactListenerInstance);
  }

  void close_collision(int &state) {
    auto world = collisionList[state];
    if (world) {
      b2Body *body = world->GetBodyList();

      while (body) {
        world->DestroyBody(body);
        body = body->GetNext();
      }
      world = nullptr;
      //      delete world[state];
    }
  }

  void Create_Static_Body_Rect(entt::registry &zone, const int &state, const entt::entity &entity, const float &x, const float &y, const Collision_Component::aabb aabb) {

    b2BodyDef bodyDef;

    bodyDef.position.Set(x, y);
    bodyDef.userData.entity_ID = (int) entity;

    auto world = collisionList[state];

    b2Body *body = world->CreateBody(&bodyDef);
    zone.emplace_or_replace<Component::Body>(entity, body);

    b2PolygonShape rect;
    rect.SetAsBox((float) aabb.hx, (float) aabb.hy);

    body->CreateFixture(&rect, 0.0f);
  }

  void Create_Static_Body_Polygon(entt::registry &zone, const int &state, const entt::entity &entity, const float &x, const float &y, const std::vector<std::vector<tmx::Vector2<float>>> &pointVecs) {

    b2BodyDef bodyDef;

    bodyDef.position.Set(x, y);
    bodyDef.userData.entity_ID = (int) entity;

    auto world = collisionList[state];
    b2Body *body = world->CreateBody(&bodyDef);
    zone.emplace_or_replace<Component::Body>(entity, body);

    for (auto pointVec: pointVecs) {
      if (!pointVec.empty()) {
        b2PolygonShape polygon;
        b2Vec2 vertices[pointVec.size()];

        for (int i = 0; i < pointVec.size(); i++) {
          vertices[i].Set(pointVec[i].x, pointVec[i].y);
        }

        auto count = (int) pointVec.size();
        polygon.Set(vertices, count);

        body->CreateFixture(&polygon, 0.0f);
      }
    }
  }

  void Add_Polygon_Fixture(b2Body *body, std::vector<tmx::Vector2<float>> &pointVec, const bool &isSensor) {
    if (pointVec.size() >= 3) {
      if (pointVec.size() <= 8) {
        b2PolygonShape polygon;
        b2Vec2 vertices[pointVec.size()];

        for (int i = 0; i < pointVec.size(); i++) {
          vertices[i].Set(pointVec[i].x, pointVec[i].y);
        }

        auto count = (int) pointVec.size();
        polygon.Set(vertices, count);
        b2FixtureDef fixture;

        fixture.isSensor = isSensor;
        fixture.shape = &polygon;
        fixture.density = 0.0f;

        body->CreateFixture(&fixture);
        return;
      }
      Utilities::Log("too many vertices in the polygon, max 8, current: " + std::to_string(pointVec.size()));
      return;
    }
    Utilities::Log("too few vertices in the polygon, min 3, current: " + std::to_string(pointVec.size()));
  }

  void Add_Circle_Fixture(b2Body *body, float radius) {
    b2CircleShape circle;
    circle.m_radius = radius;

    body->CreateFixture(&circle, 0.0f);
  }

  b2Body *Add_Static_Body(entt::registry &zone, int &state, entt::entity &entity, Component::Position &position) {
    //    auto &position = zone.get<Component::Position>(entity);
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.userData.entity_ID = (int) entity;
    bodyDef.linearDamping = 0.0f;
    bodyDef.angularDamping = 0.0f;

    auto world = collisionList[state];
    b2Body *body = world->CreateBody(&bodyDef);
    zone.emplace_or_replace<Component::Body>(entity, body);
    return body;
  }

  void Create_Static_Body(entt::registry &zone, int &state, entt::entity &entity, float &x, float &y, float radius) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x, y);
    bodyDef.userData.entity_ID = (int) entity;
    bodyDef.linearDamping = 0.0f;
    bodyDef.angularDamping = 0.0f;

    auto world = collisionList[state];
    b2Body *body = world->CreateBody(&bodyDef);
    zone.emplace_or_replace<Component::Body>(entity, body);
    auto &position = zone.get<Component::Position>(entity);

    b2CircleShape circle;
    circle.m_radius = radius;

    body->CreateFixture(&circle, 0.0f);
  }

  void Create_Kinematic_Body(entt::registry &zone, entt::entity &entity, Component::Body body, float &x, float &y, float &radius, float &mass, bool &isDynamicBody) {

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(x, y);
    bodyDef.userData.entity_ID = (int) entity;
    bodyDef.linearDamping = 0.0f;
    bodyDef.angularDamping = 0.0f;

    b2CircleShape circle;
    circle.m_radius = radius;

    b2FixtureDef bodyFixture;
    bodyFixture.shape = &circle;
    bodyFixture.density = 0.1f;//mass
    bodyFixture.friction = 0.1f;

    body.body->CreateFixture(&bodyFixture);
  }

  void Create_Dynamic_Body(entt::registry &zone, int &state, entt::entity &entity, float &x, float &y, float radius, float mass, bool &isDynamicBody) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    bodyDef.userData.entity_ID = (int) entity;
    bodyDef.linearDamping = 10.0f;
    bodyDef.angularDamping = 0.0f;

    b2World *world = collisionList[state];
    b2Body *body = world->CreateBody(&bodyDef);
    auto &bodyComponent = zone.emplace_or_replace<Component::Body>(entity, body);
    zone.emplace_or_replace<Collision_Component::Dynamic_Collider>(entity);

    b2CircleShape polygon;
    polygon.m_radius = radius;

    b2FixtureDef bodyFixture;
    bodyFixture.shape = &polygon;
    bodyFixture.density = mass / radius;
    bodyFixture.friction = 0.0f;

    body->CreateFixture(&bodyFixture);
  }

  void Set_Collision_Box(entt::registry &zone, int &state, entt::entity &entity, std::string &colliderType, Component::Position &position, Collision_Component::aabb &aabb, std::vector<std::vector<tmx::Vector2<float>>> &pointVecs, Component::Line_Segment &line, float &radius) {
    if (colliderType == "polygon") {
      Collision::Create_Static_Body_Polygon(zone, state, entity, position.x, position.y, pointVecs);
      zone.emplace_or_replace<Component::Line_Segment>(entity, line);
    } else if (colliderType == "rect") {
      Collision::Create_Static_Body_Rect(zone, state, entity, position.x, position.y, {(int) radius, (int) radius});
    } else if (colliderType == "round") {
      Collision::Create_Static_Body(zone, state, entity, position.x, position.y, radius);
    }
  }

  void Attach_Components(entt::registry &zone, int &state, entt::entity &entity, Collision_Component::Collider_Data &colliderData) {
    if (Collision_Component::houseColliders.contains(colliderData.name)) {
      auto collider = Collision_Component::houseColliders.at(colliderData.name).colliders;
      auto &position = zone.get<Component::Position>(entity);
      auto &offset = zone.get<Rendering_Components::Sprite_Offset>(entity);

      position.y += (colliderData.placementOffset - colliderData.offset.y);
      offset.y += (colliderData.placementOffset - colliderData.offset.y);

      colliderData.position.x += colliderData.offset.x;
      colliderData.position.y -= colliderData.offset.y;
      b2Body *body;
      if (zone.any_of<Component::Body>(entity)) {
        body = zone.get<Component::Body>(entity).body;
      } else {
        body = Collision::Add_Static_Body(zone, state, entity, colliderData.position);
      }
      for (int i = 0; i < collider.pointVecs.size(); ++i) {
        Collision::Add_Polygon_Fixture(body, collider.pointVecs[i], collider.isSensor[i]);
      }
      for (auto &circle: collider.circlesVecs) {
        Collision::Add_Circle_Fixture(body, colliderData.radius);
      }
    } else {
      Set_Collision_Box(zone, state, entity, colliderData.colliderType, colliderData.position, colliderData.aabb, colliderData.pointVecs, colliderData.line, colliderData.radius);
    }
    zone.emplace_or_replace<Component::Mass>(entity, 100.0f);
  }

  void Update_Collision(entt::registry &zone, int &state) {
    auto view = zone.view<Component::Position>();
    auto view2 = zone.view<Rendering_Components::Sprite_Sheet_Info>();
    b2World *world = collisionList[state];
    b2Body *body = world->GetBodyList();
    int i = 0;
    //needs to be run multiple times per frame at low frame rate otherwise it will fall behind the rest of the program
    collision_Timestep += Timer::timeStep;
    int j = 0;
    while (collision_Timestep >= timeStep) {
      collision_Timestep -= timeStep;
      world->Step(timeStep, velocityIterations, positionIterations);

      while (body) {
        if (body->GetType() == b2_dynamicBody) {
          auto entity_ID = (entt::entity) body->GetUserData().entity_ID;
          if (zone.any_of<Component::Position>(entity_ID)) {
            auto &position = view.get<Component::Position>(entity_ID);

            b2Vec2 newPosition = body->GetPosition();
            position.x = newPosition.x;
            position.y = newPosition.y;
          } else {
          }
        }
        i++;
        body = body->GetNext();
      }
      world->ClearForces();
      j++;
    }
  }

  void Collision_Routine(entt::registry &zone, int &state) {
    Update_Collision(zone, state);
  };
}// namespace Collision
