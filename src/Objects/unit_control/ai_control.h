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
	void Attack_Move(entt::registry &zone, entt::entity &entity_ID, entt::entity& target_ID, Component::Position &entityPosition, Component::Melee_Range &meleeRange, Component::Position &targetPosition, Component::Radius &targetRadius) { // maybe change to move and attack?
		if (World::zone.any_of<Component::Attacking>(entity_ID) == true) {
			return;
		}
		if (World::zone.any_of<Component::Attacking>(entity_ID) == false) {
			//calcuate the point to move to that puts in range of melee attack on every few frames
			//if it is in range, run Melee_Attack()
            //else pass that point as an update to the move order
			if (Entity_Control::Target_In_Melee_Range(zone, entityPosition, meleeRange, targetPosition, targetRadius)) { //check if center of attack rect is in the target
                auto &action = zone.get<Action_Component::Action>(entity_ID).state;
                if (action != Action_Component::attack && action != Action_Component::struck) {
                    action = Action_Component::idle;
                }
                zone.remove<Component::Mouse_Move>(entity_ID);
                zone.remove<Component::Moving>(entity_ID);
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
		for (auto unit_ID : units) {
			auto &sightBox = units.get<Component::Sight_Range>(unit_ID).sightBox;
			auto& meleeRange = units.get<Component::Melee_Range>(unit_ID);
			auto& unitPosition = units.get<Component::Position>(unit_ID);

            std::vector<entt::entity> entityData = Dynamic_Quad_Tree::Get_Nearby_Entities(zone, sightBox);

            for (auto target : entityData) {
                auto type = zone.get<Component::Entity_Type>(target);
                if (type == Component::Entity_Type::unit) {
                    if (Social_Control::Check_Relationship(zone, unit_ID, target)) {
                        auto &targetPosition = zone.get<Component::Position>(target);
                        auto &targetRadius = zone.get<Component::Radius>(target);
                        SDL_FPoint targetPoint = {targetPosition.x, targetPosition.y};

                        if (Utilities::bFPoint_FRectIntersect(targetPoint, sightBox)) {
                            zone.emplace_or_replace<Component::In_Combat>(unit_ID);
                            zone.emplace_or_replace<Component::In_Combat>(target);
                            if (zone.any_of<Component::Melee_Damage>(unit_ID)) {
                                //attack if the unit has an attack
                                Attack_Move(zone, unit_ID, target, unitPosition, meleeRange, targetPosition, targetRadius);

                                //attack text
                                std::string attack = "attack";
                                Social_Control::Interact(zone, unit_ID, attack);
                            }

                            //run away if unit does not have an attack
                            else {
                                //shriek in terror
                                std::string flee = "flee";
                                Social_Control::Interact(zone, unit_ID, flee);

                                //move in the opposite direction for a distance

                            }
                        } else {
                            zone.remove<Component::In_Combat>(unit_ID);
                        }
                        break;
                    }
                    else {

                    }
                }
            }
		}
	}

	void Update_Combat(entt::registry& zone) {
		auto units = zone.view<Component::In_Combat, Component::Attack_Speed>();
		for (auto unit_ID : units) {
			auto& attackSpeed = units.get<Component::Attack_Speed>(unit_ID);
            if (attackSpeed.counter > 0) {
                attackSpeed.counter -= (int) Timer::timeStep;
            }
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

    void Update_Player_Target_Range(entt::registry &zone) {
        //could add Component::Moving so it only updates when the entity moves
        auto view = zone.view<Component::Target_Range, Component::Position, Component::Alive>();
        for (auto entity : view) {
            auto& sight = view.get<Component::Target_Range>(entity);
            auto& position = view.get<Component::Position>(entity);
            sight.rangeBox = { position.x - (sight.range/2.0f), position.y - (sight.range/2.0f), sight.range, sight.range };
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
                Update_Player_Target_Range(zone);
                Check_For_Targets(zone);
            }
		}
		Update_Combat(zone);
	}
}