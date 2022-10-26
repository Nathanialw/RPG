#pragma once
#include "components.h"
#include "timer.h"

namespace Player_Component {
	struct Attack_Move {
		entt::entity ID;
		float x;
		float y;
		float hitRadius;
	};
}

namespace Player_Control {

	int64_t Player_Move_Poll = 0;

	void Melee_Attack(entt::registry& zone, entt::entity& entity_ID, entt::entity& target_ID, Position& targetPosition) {
		if (zone.any_of<Component::Attacking>(entity_ID) == false) { //locks out attacking until attack animation has finished
			zone.emplace_or_replace<Component::Attack>(entity_ID, target_ID, targetPosition.x, targetPosition.y);
		}
	}

	void Attack_Order(entt::registry& zone, entt::entity& entity, entt::entity &target_ID,  Component::Position &targetPosition, Component::Radius &targetRadius) {
		zone.emplace_or_replace<Component::Mouse_Move>(entity, targetPosition.x, targetPosition.y);
		zone.emplace_or_replace<Component::Moving>(entity);
		zone.emplace_or_replace<Player_Component::Attack_Move>(entity, target_ID, targetPosition.x, targetPosition.y, targetRadius.fRadius);
	}

	bool Check_If_Arrived(const float& unitX, const float& unitY, const float& destinationX, const float& destinationY) {
		float accuracy = 35.0f;
		if (unitX + accuracy > destinationX &&
			unitX - accuracy < destinationX &&
			unitY - accuracy < destinationY &&
			unitY + accuracy > destinationY) {
			return true;
		}
		else {
			return false;
		}
	}


	bool Check_hitRadius(const float& unitX, const float& unitY, float &unitRadius, const float& destinationX, const float& destinationY, float &targetRadius) {
		return false;
	}


	void Mouse_Move_To_Attack(entt::registry &zone) { //calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
		Player_Move_Poll += Timer::timeStep;
		if (Player_Move_Poll >= 200) {
			Player_Move_Poll = 0;
			auto view = zone.view<Component::Position, Component::Velocity, Component::Actions, Component::Moving, Player_Component::Attack_Move>();
			for (auto entity : view) {
				const auto& x = view.get<Component::Position>(entity);
				const auto& y = view.get<Component::Position>(entity);
				auto& act = view.get<Component::Actions>(entity);
				auto& v = view.get<Component::Velocity>(entity);
				auto& mov = view.get<Player_Component::Attack_Move>(entity);
				act.action = Component::walk;
				v.magnitude.x = v.speed * (mov.x - x.x);
				v.magnitude.y = v.speed * (mov.y - y.y);
			}
		}
	}

	void Mouse_Move_Arrived_Attack_Target(entt::registry& zone) {
		auto view = zone.view<Component::Position, Component::Velocity, Component::Actions, Player_Component::Attack_Move, Component::Moving>();
		for (auto entity : view) {
			auto& act = view.get<Component::Actions>(entity);
			auto& v = view.get<Component::Velocity>(entity);
			const auto& position = view.get<Component::Position>(entity);
			auto& target = view.get<Player_Component::Attack_Move>(entity);
			if (Check_If_Arrived(position.x, position.y, target.x, target.y)) {
				if (act.action == Component::walk) {
					v.magnitude.x = 0.0f;
					v.magnitude.y = 0.0f;
					act.action = Component::idle;

					//attack target
					Position targetPosition = { target.x, target.y };
					Melee_Attack(zone, entity, target.ID, targetPosition);

					zone.remove<Player_Component::Attack_Move>(entity);
					zone.remove<Component::Moving>(entity);
				}
			}
		}
	}

	void Move_To_Atack_Routine(entt::registry & zone) {
		Mouse_Move_To_Attack(zone);
		Mouse_Move_Arrived_Attack_Target(zone);
	}
}
