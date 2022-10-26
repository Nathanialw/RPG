#pragma once
#include "mouse_control.h"
#include "timer.h"
#include "components.h"



namespace AI {

    bool is_Ai_On = true;

	//check for targets periodically

	//use the target x,y to moiv towards it

	//if target is in range, melee attack


	void Move_Order(entt::registry& zone, entt::entity& entity, float& x, float& y) {
		zone.emplace_or_replace<Component::Mouse_Move>(entity, x, y);
		zone.emplace_or_replace<Component::Moving>(entity);
	}

	void Spell_Attack(entt::registry& zone, entt::entity& entity, float& targetX, float& targetY, const char* name) {

		if (zone.any_of<Casting>(entity) == false) { //locks out casing until cast animation has finished
			zone.emplace_or_replace<Component::Cast>(entity, targetX, targetY);
			zone.emplace_or_replace<Component::Spell_Name>(entity, name);
		}
	}

	void Melee_Attack(entt::registry& zone, entt::entity& entity_ID, entt::entity& target_ID, Position &targetPosition) {
		if (zone.any_of<Component::Attacking>(entity_ID) == false) { //locks out attacking until attack animation has finished
			zone.emplace_or_replace<Component::Attack>(entity_ID, target_ID, targetPosition.x, targetPosition.y);
		}
	}




	bool Player_In_Melee_Range(entt::registry& zone, Position &entityPosition, Melee_Range &meleeRange, Position &targetPosition, Radius &targetRadius) {
		
		float length = entityPosition.x - targetPosition.x;
		float width = entityPosition.y - targetPosition.y;

		

		if (((length * length) + (width * width)) <= ((meleeRange.meleeRange + targetRadius.fRadius) * (meleeRange.meleeRange + targetRadius.fRadius))) {
			//float range = Utilities::Get_Hypotenuse(length, width);
			//if (range <= (meleeRange.meleeRange + targetRadius.fRadius)) {
				return true;
			//}
		}
		return false;
	}



	void Attack_Move(entt::registry &zone, entt::entity &entity_ID, entt::entity& target_ID, Position &entityPosition, Melee_Range &meleeRange, Position &targetPosition, Radius &targetRadius) { // maybe change to move and attack?
		if (World::zone.any_of<Attacking>(entity_ID) == true) {
			return;
		}
		if (World::zone.any_of<Component::Attacking>(entity_ID) == false) {

			//calcuate the point to move to that puts in range of melee atack on every few frames
			// pass that point as a update to the  move order
			//if it is in range, run Melee_Attack()

			if (Player_In_Melee_Range(zone, entityPosition, meleeRange, targetPosition, targetRadius) == false) { //check if center of attack rect is in the target
				Move_Order(zone, entity_ID, targetPosition.x, targetPosition.y);
			}
			else {
				auto &action = zone.get<Actions>(entity_ID).action;
				if (action != attack) { action = idle; }
				zone.remove<Mouse_Move>(entity_ID);
				zone.remove<Moving>(entity_ID);
				Melee_Attack(zone, entity_ID, target_ID, targetPosition);				
			}
		}
			//else move to cursor
	}
		
	


	void Check_For_Targets(entt::registry& zone) {
		auto units = zone.view<Component::Sight_Range, Component::Alive, Component::Position, Component::Melee_Range>();
		auto targets = zone.view<Component::Position, Component::Radius, Component::Input>();

		for (auto unit_ID : units) {
			auto &sightBox = units.get<Component::Sight_Range>(unit_ID).sightBox;
			auto& meleeRange = units.get<Component::Melee_Range>(unit_ID);
			auto& unitPosition = units.get<Component::Position>(unit_ID);
			
			for (auto target_ID : targets) {
				auto& targetPosition = targets.get<Component::Position>(target_ID);
				auto& targetRadius = targets.get<Component::Radius>(target_ID);

				SDL_FPoint targetPoint = { targetPosition.x, targetPosition.y };
				if (Utilities::bFPoint_FRectIntersect(targetPoint, sightBox)) {
					zone.emplace_or_replace<Component::In_Combat>(unit_ID);
					zone.emplace_or_replace<Component::In_Combat>(target_ID);
					Attack_Move(zone, unit_ID, target_ID, unitPosition, meleeRange, targetPosition, targetRadius);
				}
				else {
					zone.remove<Component::In_Combat>(unit_ID);
				}
			}
		}
	}

	void Update_Combat(entt::registry& zone) {
		auto units = zone.view<Component::In_Combat, Component::Attack_Speed>();

		for (auto unit_ID : units) {
			auto& attackSpeed = units.get<Component::Attack_Speed>(unit_ID);
			attackSpeed.counter -= (int)Timer::timeStep;
		}
	}

	void Update_Sight_Box(entt::registry &zone) {
		auto view = zone.view<Component::Sight_Range, Component::Position, Component::Alive>();
		for (auto entity : view) {
			auto& sight = view.get<Component::Sight_Range>(entity).sightBox;
			auto& x = view.get<Component::Position>(entity).x;
			auto& y = view.get<Component::Position>(entity).y;

			sight = { x - 250.0f, y - 250.0f, 500.0f, 500.0f };
		}
	}

	int64_t time = 0;


	void Turn_On() {
		if (is_Ai_On == false) {
            is_Ai_On = true;
		}
		else {
            is_Ai_On = false;
		}
	}

	void Update_AI(entt::registry& zone) {
		time += Timer::timeStep;
		//std::cout << time << std::endl;
		if (time >= 100) {
			if (is_Ai_On) {
				time = 0;
				Update_Sight_Box(zone);
				Check_For_Targets(zone);	
			}
		}
		
		Update_Combat(zone);
	}
}