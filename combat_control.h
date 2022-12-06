#pragma once
#include "entt/entt.hpp"
#include "components.h"
#include "movement.h"
#include "damage_text.h"
#include "action_components.h"

namespace Combat_Control {

	int Calculate_Damage(Component::Damage& damageRange) {
		if (damageRange.minDamage == 0) {
			return 0;
			std::cout << "function Calculate_Damage() min damage range 0, divide by zero error" << std::endl;
		}
		return rand() % damageRange.maxDamage + damageRange.minDamage;
	}

    void Attack_Cast(entt::registry &zone) {
        auto view = zone.view<Component::Direction, Component::Position, Action_Component::Action, Component::Attack, Component::Velocity, Component::Attack_Speed>(entt::exclude<Component::Attacking>);
        for (auto entity : view) {
            //initiates a point and click attack
            auto& attackSpeed = view.get<Component::Attack_Speed>(entity);
            auto &action = view.get<Action_Component::Action>(entity);
                ///ensures it attacks at the end of the last frame of the attack
                /// if current attackSpeed >= attackSpeed value then subtract the attackSpeed value from the current attackSpeed counter
            if (action.state != Action_Component::attack && action.state != Action_Component::struck && action.state != Action_Component::block && attackSpeed.counter <= 0) {
                zone.emplace_or_replace<Component::In_Combat>(entity, true);
                auto &direction = view.get<Component::Direction>(entity);
                auto &angle = view.get<Component::Velocity>(entity).angle;
                auto &target = view.get<Component::Attack>(entity);
                auto &position = view.get<Component::Position>(entity);
                direction = Movement::Look_At_Target(position.x, position.y, target.targetX, target.targetY, angle);
                attackSpeed.counter = attackSpeed.period;
                ///add attack state for render state
                action.state = Action_Component::attack;
                action.frame = 0;
                zone.emplace_or_replace<Component::Attacking>(entity, target.target_ID);
                zone.remove<Component::Attack>(entity);
            }
            else {
                zone.remove<Component::Attack>(entity);
            }
        }
        //create SDL_FRect in front of unit, size from a size component on weapon
    }

    void Attack_Target(entt::registry &zone) {
        auto view = zone.view<Component::Attacking, Action_Component::Action, Component::Melee_Damage>();
        for (auto entity : view) {
            auto& action = view.get<Action_Component::Action>(entity);
            if (action.state == Action_Component::attack) {
                ///ensures it attacks at the end of the last frame of the attack
                if (action.frameState == Action_Component::last) {
                    //executes a point and click attack
                    auto &target_ID = view.get<Component::Attacking>(entity).target_ID;
                    auto &meleeDamage = view.get<Component::Melee_Damage>(entity);
                        /// calculate damage and show for player
                    Component::Damage damageRange = {meleeDamage.minDamage, meleeDamage.maxDamage};
                    int damage = Calculate_Damage(damageRange);

                    if (zone.any_of<Component::Input>(entity)) {
                        Damage_Text::Add_To_Scrolling_Damage(zone, entity, target_ID, damage);
                    }

                    auto &struck = zone.get_or_emplace<Component::Struck>(target_ID);

                    if (meleeDamage.critical) {
                        struck.critical = true;
                        auto &targetAction = zone.get_or_emplace<Action_Component::Action>(target_ID);
                        targetAction.frame = 0;
                        targetAction.state = Action_Component::struck;
                    }
                    struck.struck += damage;
                    zone.remove<Component::Attacking>(entity);
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
        auto view = zone.view<Component::Struck, Action_Component::Action, Rendering_Components::Sprite_Sheet_Info>();
        for (auto entity : view) {
            auto &action = view.get<Action_Component::Action>(entity);
            auto &sheetData = view.get<Rendering_Components::Sprite_Sheet_Info>(entity);
            if (action.state == Action_Component::dying) {
                zone.remove<Component::Struck>(entity);
            }
            if (sheetData.flareSpritesheet) {
                if (action.frameState == Action_Component::last) {
                        ///should not return to idle, should go into an "idle-combat" mode
                    zone.remove<Component::Struck>(entity);
                }
            }
            else if (sheetData.sheetData) {
                if (action.frameState == Action_Component::last) {
                        ///should not return to idle, should go into an "idle-combat" mode
                    zone.remove<Component::Struck>(entity);
                }
            }
            else {
                Utilities::Log("Struck_Updater(entt::registry &zone) - Both NULL passthrough error");
            }
        }
    }

    void Update_Attacks(entt::registry& zone) {
        AttackSpeed_Updater(zone);
        Struck_Updater(zone);
        Attack_Cast(zone);
        Attack_Target(zone);
        Reset_Alert(zone);
        Alert_When_Struck(zone);
    }
}
