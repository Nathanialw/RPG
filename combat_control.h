#pragma once
#include "entt.hpp"
#include "components.h"
#include "movement.h"
#include "damage_text.h"

namespace Combat_Control {

	int Calculate_Damage(Component::Damage& damageRange) {
		if (damageRange.minDamage == 0) {
			return 0;
			std::cout << "function Calculate_Damage() min damage range 0, divide by zero error" << std::endl;
		}
		return rand() % damageRange.maxDamage + damageRange.minDamage;
	}

    void Attack_Cast(entt::registry &zone) {
        auto view = zone.view<Component::Direction, Component::Position, Component::Actions, Component::Attack, Component::Velocity, Component::Attack_Speed>(entt::exclude<Component::Attacking>);
        for (auto entity : view) {
            //initiates a point and click attack
            auto& attackSpeed = view.get<Component::Attack_Speed>(entity);
            auto &act = view.get<Component::Actions>(entity);
                ///ensures it attacks at the end of the last frame of the attack
                /// if current attackSpeed >= attackSpeed value then subtract the attackSpeed value from the current attackSpeed counter
            if (act.action != Component::attack && act.action != Component::struck & attackSpeed.counter <= 0) {
                zone.emplace_or_replace<Component::In_Combat>(entity, true);
                auto &direction = view.get<Component::Direction>(entity);
                auto &angle = view.get<Component::Velocity>(entity).angle;
                auto &target = view.get<Component::Attack>(entity);
                auto &position = view.get<Component::Position>(entity);
                direction = Movement::Look_At_Target(position.x, position.y, target.targetX, target.targetY, angle);
                attackSpeed.counter = attackSpeed.period;
                ///add attack action for render state
                act.action = Component::attack;
                act.frameCount[act.action].currentFrame = 0;
                zone.emplace_or_replace<Component::Attacking>(entity, target.target_ID);
                zone.remove<Component::Attack>(entity);
            }
        }
        //create SDL_FRect in front of unit, size from a size component on weapon
    }

    void Attack_Target(entt::registry &zone) {
        auto view = zone.view<Component::Attacking, Component::Actions, Component::Melee_Damage>();
        for (auto entity : view) {
            auto& act = view.get<Component::Actions>(entity);
            if (act.action == Component::attack) {
                    ///ensures it attacks at the end of the last frame of the attack
                if (act.frameCount[act.action].currentFrame >= (act.frameCount[act.action].NumFrames - 1)) {
                    //executes a point and click attack
                    auto& target = view.get<Component::Attacking>(entity).target_ID;
                    auto& meleeDamage = view.get<Component::Melee_Damage>(entity);
                    Component::Damage damageRange = { meleeDamage.minDamage, meleeDamage.maxDamage };
                    int damage = Calculate_Damage(damageRange);
                    if (zone.any_of<Component::Input>(entity) ) {
                        Damage_Text::Add_To_Scrolling_Damage(zone, entity, target, damage);
                    }
                    auto& struck = zone.get_or_emplace<Component::Struck>(target).struck;
                    auto& action = zone.get_or_emplace<Component::Actions>(target);
                    action.action = Component::struck;
                    action.frameCount[Component::struck].currentFrame = 0;
                    struck += damage;
                    //create_attack(position, direction);
                    zone.remove<Component::Attacking>(entity);
                    //act.frameCount[act.action].currentFrame = 0;
                }
            }
            else {
                zone.remove<Component::Attacking>(entity);
            }
        }
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
    SDL_FRect Create_Attack_Box(Component::Position& position, Component::Direction& direction) {
        switch (direction) {
            case Component::Direction::N: return { position.x - 15, position.y - 55, 30, 40 };
            case Component::Direction::S: return { position.x - 15, position.y + 15, 30, 40 };
            case Component::Direction::E: return { position.x + 15, position.y - 15, 40, 30 };
            case Component::Direction::W: return { position.x - 55, position.y - 15, 40, 30 };
            case Component::Direction::NW :return { position.x - 45, position.y - 45, 30, 30 };
            case Component::Direction::NE :return { position.x + 15, position.y - 45, 30, 30 };
            case Component::Direction::SW :return { position.x - 45, position.y + 15, 30, 30 };
            case Component::Direction::SE :return { position.x + 15, position.y + 15, 30, 30 };
        }
        std::cout << "Weapons::Attack_Direction() passthrough error" << std::endl;
        return {0,0,0,0};
    }

    void Attack_Box_Destroy(entt::registry &zone) {
        auto view = zone.view<Component::Attack_Box_Duration>();
        for (auto entity : view) {
            auto& time = view.get<Component::Attack_Box_Duration>(entity).lifeTime;
            auto& count = view.get<Component::Attack_Box_Duration>(entity).count;
            count += Timer::timeStep;
            if (count >= time) {
                zone.destroy(entity);
            }
        }
    }

    void Create_AoE_Attack(entt::registry &zone, Component::Position& position, Component::Direction& direction) {
        SDL_FRect attackRect = Create_Attack_Box(position, direction);

        auto aoeAttack = zone.create();
        zone.emplace<Component::Damage>(aoeAttack, 1, 10);
        zone.emplace<Component::Attack_Box_Duration>(aoeAttack, 0, 0);
        zone.emplace<Component::Mass>(aoeAttack, 500.0f);
        zone.emplace<Component::Weapon_Size>(aoeAttack, attackRect.x, attackRect.y, attackRect.w, attackRect.h); //set x, y to in front of char when he attacks
        zone.emplace<Component::Position>(aoeAttack, position);
        zone.emplace<Component::Alive>(aoeAttack, true);
        zone.emplace<Component::Entity_Type>(aoeAttack, Component::Entity_Type::melee);
    }

    void AttackSpeed_Updater(entt::registry &zone) {
        auto view = zone.view<Component::Attack_Speed>();
        for (auto entity : view) {
            auto& attackSpeed = view.get<Component::Attack_Speed>(entity);
            if (attackSpeed.counter >= 0) {
                attackSpeed.counter -= (int)Timer::timeStep;
            }
        }
    }

    void Struck_Updater(entt::registry &zone){
        auto view = World::zone.view<Component::Struck, Component::Actions>();
        for (auto entity : view) {
            auto &action = view.get<Component::Actions>(entity);
            if (action.action == Component::dead) {
                zone.remove<Component::Struck>(entity);
            }
            else if (action.frameCount[action.action].currentFrame >= action.frameCount[action.action].NumFrames - 1) {
                action.action = Component::idle;
                zone.remove<Component::Struck>(entity);
            }
        }
    }

    void Update_Attacks(entt::registry& zone) {
        Attack_Cast(zone);
        Attack_Target(zone);
        Reset_Alert(zone);
        Alert_When_Struck(zone);
        Struck_Updater(zone);
    }
}
