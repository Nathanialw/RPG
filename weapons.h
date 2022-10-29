#pragma once
#include "components.h"
#include "world.h"
#include "timer.h"
#include "movement.h"
#include "combat_control.h"
#include "damage_text.h"

namespace Weapons {

	//take input of unit Radius and Weapon_Size to fill this out for any wielder and any weapon
	SDL_FRect Create_Attack_Box(Position& position, Component::Direction& direction) {
		switch (direction) {
		case Direction::N: return { position.x - 15, position.y - 55, 30, 40 };
		case Direction::S: return { position.x - 15, position.y + 15, 30, 40 };
		case Direction::E: return { position.x + 15, position.y - 15, 40, 30 };
		case Direction::W: return { position.x - 55, position.y - 15, 40, 30 };
		case Direction::NW :return { position.x - 45, position.y - 45, 30, 30 };
		case Direction::NE :return { position.x + 15, position.y - 45, 30, 30 };
		case Direction::SW :return { position.x - 45, position.y + 15, 30, 30 };
		case Direction::SE :return { position.x + 15, position.y + 15, 30, 30 };
		}
		std::cout << "Weapons::Attack_Direction() passthrough error" << std::endl;
		return {0,0,0,0};
	}


	void create_attack(Position& position, Component::Direction& direction) {
		SDL_FRect attackRect = Create_Attack_Box(position, direction);

		auto weapon = World::zone.create();
		
		World::zone.emplace<Component::Damage>(weapon, 1, 10);
		World::zone.emplace<Component::Attack_Box_Duration>(weapon, 0, 0);
		World::zone.emplace<Component::Mass>(weapon, 500.0f);
		World::zone.emplace<Component::Weapon_Size>(weapon, attackRect.x, attackRect.y, attackRect.w, attackRect.h); //set x, y to in front of char when he attacks
		World::zone.emplace<Component::Position>(weapon, position);
		World::zone.emplace<Component::Alive>(weapon, true);
		World::zone.emplace<Component::Entity_Type>(weapon, Entity_Type::melee);
	}

	void Attack_Cast() {
		auto view = World::zone.view<Direction, Position, Actions, Attack, Velocity, Attack_Speed>(entt::exclude<Attacking>);
		for (auto entity : view) {
			auto& attackSpeed = view.get<Attack_Speed>(entity);			
			if (attackSpeed.counter <= 0) {/// if current attackSpeed >= attackSpeed value then subtract the attackSpeed value from the current attackSpeed counter
				World::zone.emplace_or_replace<Component::In_Combat>(entity);
				
				auto& act = view.get<Actions>(entity);
				auto& direction = view.get<Direction>(entity);
				auto& angle = view.get<Velocity>(entity).angle;
				auto& target = view.get<Attack>(entity);
				auto& position = view.get<Position>(entity);

				direction = Movement::Look_At_Target(position.x, position.y, target.targetX, target.targetY, angle);
				attackSpeed.counter = attackSpeed.period;
				act.action = attack;
				act.frameCount[act.action].currentFrame = 0;
				World::zone.emplace_or_replace<Attacking>(entity, target.target_ID);
				World::zone.remove<Attack>(entity);
			}
		}
		//create SDL_FRect in front of unit, size from a size component on weapon
	}

	void AttackSpeed_Updater() {
		auto view = World::zone.view<Attack_Speed>();
		for (auto entity : view) {
			auto& attackSpeed = view.get<Attack_Speed>(entity);
			if (attackSpeed.counter >= 0) {
				attackSpeed.counter -= (int)Timer::timeStep;
			}
		}
	}

	void Attack_Box_Destroy() {
		auto view = World::zone.view<Attack_Box_Duration>();
		for (auto entity : view) {
			auto& time = view.get<Attack_Box_Duration>(entity).lifeTime;
			auto& count = view.get<Attack_Box_Duration>(entity).count;
			count += Timer::timeStep;
			if (count >= time) {
				World::zone.destroy(entity);
			}
		}
	}


	void Attack_Target(entt::registry &zone) {
		auto view = World::zone.view<Attacking, Actions, Melee_Damage>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			if (act.action == attack) {
				if (act.frameCount[act.action].currentFrame >= act.frameCount[act.action].NumFrames) {
					auto& target = view.get<Attacking>(entity).target_ID;
					auto& meleeDamage = view.get<Melee_Damage>(entity);
					Damage damageRange = { meleeDamage.minDamage, meleeDamage.maxDamage };
					int damage = Combat_Control::Calculate_Damage(damageRange);


                    if (zone.any_of<Input>(entity) ) {
                        Damage_Text::Add_To_Scrolling_Damage(zone, entity, target, damage);
                    }
					//std::cout << damage << std::endl;

					auto& struck = zone.get_or_emplace<Struck>(target);
					struck.struck += damage;

					//create_attack(position, direction);
					World::zone.remove<Attacking>(entity);

					act.action = idle;
					//act.frameCount[act.action].currentFrame = 0;
				}
				
			}
			else {
				World::zone.remove<Attacking>(entity);
			}
		}
	}


	void Update_Attacks(entt::registry& zone) {
		
		Attack_Box_Destroy();
		Attack_Target(zone);
		Attack_Cast();
	}
}