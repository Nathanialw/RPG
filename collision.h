#pragma once
#include "entt/entt.hpp"
//#include "include/box2d/box2d.h"
#include "components.h"
#include "scene.h"
#include "tmxlite/Types.hpp"
#include "tmxlite/Object.hpp"

namespace Collision {

	b2World* world;
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

    struct aabb {
        int hx = 0;
        int hy = 0;
    };

	void init_Collison() {

		b2Vec2 gravity;
		gravity.SetZero();
		world = new b2World(gravity); //no gravity
	}

	void Create_Static_Body_Rect(entt::registry &zone, entt::entity& entity, float& x, float& y, Collision::aabb aabb) {

        b2BodyDef bodyDef;

		bodyDef.position.Set(x, y);
		bodyDef.userData.entity_ID = (int)entity;

		b2Body* body = world->CreateBody(&bodyDef);
		zone.emplace<Component::Body>(entity, body);

        b2PolygonShape rect;
        rect.SetAsBox(aabb.hx, aabb.hy);

        body->CreateFixture(&rect, 0.0f);
	}

    void Create_Static_Body_Polygon(entt::registry &zone, entt::entity& entity, float& x, float& y, std::vector<std::vector<tmx::Vector2<float>>> pointVecs) {

        b2BodyDef bodyDef;

		bodyDef.position.Set(x, y);
		bodyDef.userData.entity_ID = (int)entity;

		b2Body* body = world->CreateBody(&bodyDef);
		zone.emplace<Component::Body>(entity, body);

        for (auto pointVec : pointVecs) {
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

    void Create_Static_Body(entt::registry &zone, entt::entity& entity, float& x, float& y, float& radius) {
        b2BodyDef bodyDef;

        bodyDef.position.Set(x, y);
        bodyDef.userData.entity_ID = (int)entity;

        b2Body* body = world->CreateBody(&bodyDef);
        zone.emplace<Component::Body>(entity, body);

        b2CircleShape circle;
        circle.m_radius = radius;

        body->CreateFixture(&circle, 0.0f);
    }

	void Create_Kinematic_Body(entt::registry& zone, entt::entity& entity, Component::Body &body, float& x, float& y, float& radius, float& mass, bool& isDynamicBody) {

		b2BodyDef bodyDef;
		bodyDef.type = b2_kinematicBody;
		bodyDef.position.Set(x, y);
		bodyDef.userData.entity_ID = (int)entity;
		bodyDef.linearDamping = 1000.0f;
		bodyDef.angularDamping = 1000.0f;

		b2Body* rigidBody = world->CreateBody(&bodyDef);
		body.rigidBody = rigidBody;

		b2CircleShape circle;
		circle.m_radius = radius;

		b2FixtureDef bodyFixture;
		bodyFixture.shape = &circle;
		bodyFixture.density = 100.0f;
		bodyFixture.friction = 0.3f;
		bodyFixture.filter.groupIndex = 2;

		rigidBody->CreateFixture(&bodyFixture);
	}

	void Create_Dynamic_Body(entt::registry& zone, entt::entity &entity, float &x, float &y, float &radius, float &mass, bool &isDynamicBody) {

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		bodyDef.userData.entity_ID = (int)entity;
		bodyDef.linearDamping = 10.0f;
		bodyDef.angularDamping = 10.0f;

		b2Body* body = world->CreateBody(&bodyDef);
		auto &bodyComponent = zone.emplace<Component::Body>(entity, body);

		b2CircleShape polygon;
        polygon.m_radius = radius;

        b2FixtureDef bodyFixture;
		bodyFixture.shape = &polygon;
		bodyFixture.density = 100.0f;
		bodyFixture.friction = 0.3f;

		body->CreateFixture(&bodyFixture);
	}

    /// only really works for static collisions
	void Update_Collision(entt::registry& zone) {

		auto view = zone.view<Component::Position, Component::Body>();
		b2Body* body = world->GetBodyList();

		world->Step(timeStep, velocityIterations, positionIterations);

		while (body != NULL) {
			auto entity_ID = (entt::entity)body->GetUserData().entity_ID;
			auto &pBody = view.get<Component::Body>(entity_ID).body;
			auto &position = view.get<Component::Position>(entity_ID);

			b2Vec2 newPosition = body->GetPosition();
			position.x = newPosition.x;
			position.y = newPosition.y;

			body = body->GetNext();
		}
	}
}
