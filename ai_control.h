#pragma once
#include "mouse_control.h"
#include "timer.h"
#include "components.h"
#include "ai_components.h"
#include "entity_control.h"

namespace AI {
    bool b_AI = true;

	//check for targets periodically
	//use the target x,y to move towards it
	//if target is in range, melee attack
	void Attack_Move(entt::registry &zone, entt::entity &entity_ID, entt::entity& target_ID, Position &entityPosition, Melee_Range &meleeRange, Position &targetPosition, Radius &targetRadius) { // maybe change to move and attack?
		if (World::zone.any_of<Attacking>(entity_ID) == true) {
			return;
		}
		if (World::zone.any_of<Component::Attacking>(entity_ID) == false) {
			//calcuate the point to move to that puts in range of melee attack on every few frames
			//if it is in range, run Melee_Attack()
            //else pass that point as an update to the move order
			if (Entity_Control::Target_In_Melee_Range(zone, entityPosition, meleeRange, targetPosition, targetRadius)) { //check if center of attack rect is in the target
                auto &action = zone.get<Actions>(entity_ID).action;
                if (action != attack) {
                    action = idle;
                }
                zone.remove<Mouse_Move>(entity_ID);
                zone.remove<Moving>(entity_ID);
                Entity_Control::Melee_Attack(zone, entity_ID, target_ID, targetPosition);
			}
			else {
                Entity_Control::Move_Order(zone, entity_ID, targetPosition.x, targetPosition.y);
			}
		}
        //else move to cursor
	}

	void Check_For_Targets(entt::registry& zone) {
		auto units = zone.view<Component::Sight_Range, Component::Alive, Component::Position, Component::Melee_Range>();
        //currently specifically looks for a player as a target using input component
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
        //could add Component::Moving so it only updates when the entity moves
		auto view = zone.view<Component::Sight_Range, Component::Position, Component::Alive>();
		for (auto entity : view) {
			auto& sight = view.get<Component::Sight_Range>(entity);
			auto& position = view.get<Component::Position>(entity);
			sight.sightBox = { position.x - (sight.range/2.0f), position.y - (sight.range/2.0f), sight.range, sight.range };
		}
	}

	void Turn_On() {
		if (b_AI == false) {
            b_AI = true;
		}
		else {
            b_AI = false;
		}
	}

    int64_t time = 0;
	void Update_AI(entt::registry& zone) {
        if (b_AI) {
		    time += Timer::timeStep;
		    //std::cout << time << std::endl;
            if (time >= 100) {
                time = 0;
                Update_Sight_Box(zone);
                Check_For_Targets(zone);
            }
		}
		Update_Combat(zone);
	}
}