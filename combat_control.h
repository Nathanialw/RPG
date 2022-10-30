#pragma once
#include "entt.hpp"
#include "components.h"
#include "movement.h"
#include "damage_text.h"

namespace Combat_Control {

	int Calculate_Damage(Damage& damageRange) {
		if (damageRange.minDamage == 0) {
			return 0;
			std::cout << "function Calculate_Damage() min damage range 0, divide by zero error" << std::endl;
		}
		return rand() % damageRange.maxDamage + damageRange.minDamage;
	}

    void Attack_Target(entt::registry &zone) {
        auto view = zone.view<Attacking, Actions, Melee_Damage>();
        for (auto entity : view) {
            auto& act = view.get<Actions>(entity);
            if (act.action == attack) {
                if (act.frameCount[act.action].currentFrame >= act.frameCount[act.action].NumFrames) {
                    //executes a point and click attack
                    auto& target = view.get<Attacking>(entity).target_ID;
                    auto& meleeDamage = view.get<Melee_Damage>(entity);
                    Damage damageRange = { meleeDamage.minDamage, meleeDamage.maxDamage };
                    int damage = Calculate_Damage(damageRange);
                    if (zone.any_of<Input>(entity) ) {
                        Damage_Text::Add_To_Scrolling_Damage(zone, entity, target, damage);
                    }
                    auto& struck = zone.get_or_emplace<Struck>(target);
                    struck.struck += damage;
                    //create_attack(position, direction);
                    zone.remove<Attacking>(entity);
                    act.action = idle;
                    //act.frameCount[act.action].currentFrame = 0;
                }
            }
            else {
                zone.remove<Attacking>(entity);
            }
        }
    }

    void Attack_Cast(entt::registry &zone) {
        auto view = zone.view<Direction, Position, Actions, Attack, Velocity, Attack_Speed>(entt::exclude<Attacking>);
        for (auto entity : view) {
            //initiates a point and click attack
            auto& attackSpeed = view.get<Attack_Speed>(entity);
            if (attackSpeed.counter <= 0) {/// if current attackSpeed >= attackSpeed value then subtract the attackSpeed value from the current attackSpeed counter
                zone.emplace_or_replace<Component::In_Combat>(entity, true);

                auto& act = view.get<Actions>(entity);
                auto& direction = view.get<Direction>(entity);
                auto& angle = view.get<Velocity>(entity).angle;
                auto& target = view.get<Attack>(entity);
                auto& position = view.get<Position>(entity);

                direction = Movement::Look_At_Target(position.x, position.y, target.targetX, target.targetY, angle);
                attackSpeed.counter = attackSpeed.period;
                act.action = attack;
                act.frameCount[act.action].currentFrame = 0;
                zone.emplace_or_replace<Attacking>(entity, target.target_ID);
                zone.remove<Attack>(entity);
            }
        }
        //create SDL_FRect in front of unit, size from a size component on weapon
    }

    void Alert_When_Struck (entt::registry &zone) {
        auto view = zone.view<Component::Sight_Range, Component::Struck>(entt::exclude<Component::In_Combat, Component::Alert>);
        //expands sight range
        for (auto entity : view) {
            auto &sight = view.get<Component::Sight_Range>(entity);
            zone.emplace_or_replace<Component::Alert>(entity, sight.range);
            sight.range *= 2.0f;
        }
    }

    void Reset_Alert (entt::registry &zone) {
        auto view = zone.view<Component::Alert, Component::In_Combat, Component::Sight_Range>();
        for (auto entity : view) {
            //resets sight range after entering combat
            auto &sight = view.get<Component::Sight_Range>(entity);
            auto &alert = view.get<Component::Alert>(entity);
            sight.range = alert.sightRange;
            zone.remove<Component::Alert>(entity);
        }
    }

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

    void Attack_Box_Destroy(entt::registry &zone) {
        auto view = zone.view<Attack_Box_Duration>();
        for (auto entity : view) {
            auto& time = view.get<Attack_Box_Duration>(entity).lifeTime;
            auto& count = view.get<Attack_Box_Duration>(entity).count;
            count += Timer::timeStep;
            if (count >= time) {
                zone.destroy(entity);
            }
        }
    }

    void Create_AoE_Attack(entt::registry &zone, Position& position, Component::Direction& direction) {
        SDL_FRect attackRect = Create_Attack_Box(position, direction);

        auto aoeAttack = zone.create();
        zone.emplace<Component::Damage>(aoeAttack, 1, 10);
        zone.emplace<Component::Attack_Box_Duration>(aoeAttack, 0, 0);
        zone.emplace<Component::Mass>(aoeAttack, 500.0f);
        zone.emplace<Component::Weapon_Size>(aoeAttack, attackRect.x, attackRect.y, attackRect.w, attackRect.h); //set x, y to in front of char when he attacks
        zone.emplace<Component::Position>(aoeAttack, position);
        zone.emplace<Component::Alive>(aoeAttack, true);
        zone.emplace<Component::Entity_Type>(aoeAttack, Entity_Type::melee);
    }

    void AttackSpeed_Updater(entt::registry &zone) {
        auto view = zone.view<Attack_Speed>();
        for (auto entity : view) {
            auto& attackSpeed = view.get<Attack_Speed>(entity);
            if (attackSpeed.counter >= 0) {
                attackSpeed.counter -= (int)Timer::timeStep;
            }
        }
    }

    void Update_Attacks(entt::registry& zone) {
        Attack_Cast(zone);
        Attack_Target(zone);
        Reset_Alert(zone);
        Alert_When_Struck(zone);
    }
}
