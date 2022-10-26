#pragma once
#include "entt.hpp"
//#include "include/box2d/box2d.h"
#include "components.h"
#include "scene.h"

namespace Collision {


	b2World* world;
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;



	void init_Collison() {

		b2Vec2 gravity;
		gravity.SetZero();
		world = new b2World(gravity); //no gravity
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

		b2CircleShape circle;
		circle.m_radius = radius;

		b2FixtureDef bodyFixture;
		bodyFixture.shape = &circle;
		bodyFixture.density = 100.0f;
		bodyFixture.friction = 0.3f;
		//bodyFixture.restitution = 10.0f;
		//bodyFixture.filter.groupIndex = 1;


		body->CreateFixture(&bodyFixture);
		//Create_Kinematic_Body(zone, entity, bodyComponent, x, y, radius, mass, isDynamicBody);
	}



	void Update_Collision(entt::registry& zone) {

		auto view = zone.view<Component::Position, Component::Body>();
		b2Body* body = world->GetBodyList();

		world->Step(timeStep, velocityIterations, positionIterations);

		while (body != NULL) {

			auto entity_ID = (entt::entity)body->GetUserData().entity_ID;
			auto &pBody = view.get<Component::Body>(entity_ID).body;
			b2Vec2 newPosition = body->GetPosition();

			auto &position = view.get<Component::Position>(entity_ID);
			position.x = newPosition.x;
			position.y = newPosition.y;

			body = body->GetNext();
		}


	}










}
