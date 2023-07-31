#pragma once

#include "entt/entt.hpp"
#include "components.h"
#include "scene.h"
#include "tmxlite/Object.hpp"
#include "dynamic_quad_tree.h"

namespace Collision_Component {

  struct Dynamic_Collider {
    bool awake = true;
  };

  struct Dynamic_Collision {
    float x = 0;
    float y = 0;
  };
}

namespace Collision {

  b2World *world;
  const float timeStep = 1.0f / 5.0f;
  const float M2P = 60.0f;
  const float P2M = 1.0f / M2P;
  int32 velocityIterations = 1;
  int32 positionIterations = 1;

  struct aabb {
    int hx = 0;
    int hy = 0;
  };

  void init_Collison() {
    b2Vec2 gravity;
    gravity.SetZero();
    world = new b2World(gravity); //no gravity
    world->SetAutoClearForces(false);
  }

  void Create_Static_Body_Rect(entt::registry &zone, entt::entity &entity, float &x, float &y, Collision::aabb aabb) {

    b2BodyDef bodyDef;

    bodyDef.position.Set(x, y);
    bodyDef.userData.entity_ID = (int) entity;

    b2Body *body = world->CreateBody(&bodyDef);
    zone.emplace<Component::Body>(entity, body);

    b2PolygonShape rect;
    rect.SetAsBox(aabb.hx, aabb.hy);

    body->CreateFixture(&rect, 0.0f);
  }

  void Create_Static_Body_Polygon(entt::registry &zone, entt::entity &entity, float &x, float &y, std::vector<std::vector<tmx::Vector2<float>>> pointVecs) {

    b2BodyDef bodyDef;

    bodyDef.position.Set(x, y);
    bodyDef.userData.entity_ID = (int) entity;

    b2Body *body = world->CreateBody(&bodyDef);
    zone.emplace<Component::Body>(entity, body);

    for (auto pointVec: pointVecs) {
      if (pointVec.size() > 0) {
        b2PolygonShape rect;
        b2Vec2 vertices[pointVec.size()];

        for (int i = 0; i < pointVec.size(); i++) {
          vertices[i].Set(pointVec[i].x, pointVec[i].y);
        }

        int32 count = pointVec.size();
        rect.Set(vertices, count);

        body->CreateFixture(&rect, 0.0f);
      }
    }
  }

  void Create_Static_Body(entt::registry &zone, entt::entity &entity, float &x, float &y, float radius) {
    b2BodyDef bodyDef;

    bodyDef.position.Set(x, y);
    bodyDef.userData.entity_ID = (int) entity;

    b2Body *body = world->CreateBody(&bodyDef);
    zone.emplace<Component::Body>(entity, body);

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
    bodyFixture.density = 0.1f;     //mass
    bodyFixture.friction = 0.1f;

    body.body->CreateFixture(&bodyFixture);
  }

  void Create_Dynamic_Body(entt::registry &zone, entt::entity &entity, float &x, float &y, float radius, float mass, bool &isDynamicBody) {

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    bodyDef.userData.entity_ID = (int) entity;
    bodyDef.linearDamping = 10.0f;
    bodyDef.angularDamping = 0.0f;

    b2Body *body = world->CreateBody(&bodyDef);
    auto &bodyComponent = zone.emplace<Component::Body>(entity, body);

    b2CircleShape polygon;
    polygon.m_radius = radius;

    b2FixtureDef bodyFixture;
    bodyFixture.shape = &polygon;
    bodyFixture.density = mass / radius;
    bodyFixture.friction = 0.0f;

    //        bodyFixture.filter.groupIndex = -8;
    body->CreateFixture(&bodyFixture);
    //        Create_Kinematic_Body(zone, entity, bodyComponent, x, y,radius, mass, isDynamicBody);
  }

  void Dynamic_Collisions(entt::registry &zone) {

    auto view = zone.view<Collision_Component::Dynamic_Collider, Component::Position, Component::Radius, Component::Mass>();
    for (auto entity: view) {
      auto &awake = view.get<Collision_Component::Dynamic_Collider>(entity);
      //            if (awake.awake == true) {
      auto &position = view.get<Component::Position>(entity);
      auto &radius = view.get<Component::Radius>(entity).fRadius;
      auto &mass = view.get<Component::Mass>(entity).fKilos;

      auto positionRect = Utilities::Get_FRect_From_Point_Radius(radius, position.x, position.y);
      //get all entities colliding with the entity's rect
      std::vector<entt::entity> list = Dynamic_Quad_Tree::Get_Nearby_Entities(zone, positionRect);

      //test each entity for a circle collision and apply a collision component to accumulate the resolution data
      for (auto collision_ID: list) {
        //so it doesn't colled with itself
        if (entity != collision_ID) {
          if (zone.any_of<Collision_Component::Dynamic_Collider>(collision_ID)) {
            auto &collision_Position = zone.get<Component::Position>(collision_ID);
            auto &collision_Radius = zone.get<Component::Radius>(collision_ID).fRadius;
            //check radius distance
            float fx = position.x - collision_Position.x;
            float fy = position.y - collision_Position.y;
            float fDistance = (fx * fx) + (fy * fy);

            if (fDistance <= ((radius + collision_Radius) * (radius + collision_Radius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors
              fDistance = (float) sqrtf(fDistance);
              if (fDistance == 0.0f) { fDistance = 0.0001; }
              float fOverlap = fDistance - (radius + collision_Radius);

              f2 resolver = {};
              resolver.x = fOverlap * (position.x - collision_Position.x) / fDistance;
              resolver.y = fOverlap * (position.y - collision_Position.y) / fDistance;

              auto &collision_Mass = zone.get<Component::Mass>(collision_ID).fKilos;
              float fTotalmass = mass + collision_Mass;
              float fNomalizedMassA = (mass / fTotalmass);
              float fNomalizedMassB = (collision_Mass / fTotalmass);

              float resolverX = (resolver.x * fNomalizedMassB);
              float resolverY = (resolver.y * fNomalizedMassB);
              auto &collision_ResolverA = zone.emplace_or_replace<Collision_Component::Dynamic_Collision>(entity);
              collision_ResolverA.x += resolverX;
              collision_ResolverA.y += resolverY;

              float bresolverX = (resolver.x * fNomalizedMassA);
              float bresolverY = (resolver.y * fNomalizedMassA);
              auto &collision_ResolverB = zone.emplace_or_replace<Collision_Component::Dynamic_Collision>(collision_ID);
              collision_ResolverB.x -= bresolverX;
              collision_ResolverB.y -= bresolverY;
            }
          }
        }
        //                }
      }
    }
  }

  void Resolve_Dynamic_Collisions(entt::registry &zone) {
    int i = 0;
    auto view = zone.view<Collision_Component::Dynamic_Collider, Collision_Component::Dynamic_Collision, Component::Body>();
    for (auto entity: view) {
      i++;
      auto &body = view.get<Component::Body>(entity);
      auto &collision = view.get<Collision_Component::Dynamic_Collision>(entity);

      b2Vec2 bodyPosition = {0, 0};
      body.body->SetLinearVelocity(bodyPosition);
      bodyPosition.x = body.body->GetTransform().p.x - collision.x;
      bodyPosition.y = body.body->GetTransform().p.y - collision.y;

      body.body->SetTransform(bodyPosition, 0);
      zone.remove<Collision_Component::Dynamic_Collision>(entity);
    }
    Utilities::Log(i);
  }

  float collision_Timestep = 0.0f;

  void Update_Collision(entt::registry &zone) {
    auto view = zone.view<Component::Position, Component::Body>();
    b2Body *body = world->GetBodyList();
    int i = 0;
    //needs to be run multiple times per frame at low frame rate otherwise it will fall behind the rest of the program
    collision_Timestep += Timer::timeStep;
    while (collision_Timestep > 0.0f) {
      collision_Timestep -= timeStep;
      world->Step(timeStep, velocityIterations, positionIterations);

      while (body != NULL) {
        if (body->GetType() == b2_dynamicBody) {
          auto entity_ID = (entt::entity) body->GetUserData().entity_ID;
          auto &position = view.get<Component::Position>(entity_ID);

          b2Vec2 newPosition = body->GetPosition();
          position.x = newPosition.x;
          position.y = newPosition.y;
        }
        i++;
        body = body->GetNext();
      }
      world->ClearForces();
    }
    Debug::collisionChecks = i;
  }

  void Collision_Routine(entt::registry &zone) {
    //        Dynamic_Collisions(zone);
    //        Resolve_Dynamic_Collisions(zone);
    Update_Collision(zone);
  };
}
