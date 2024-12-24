#pragma once

#include "Input/mouse_control.h"
#include "Maps/Pathing/a_star.h"
#include "Maps/World/world.h"
#include "Movement/movement_functions.h"
#include "Objects/Social/social_control.h"
#include "ai_components.h"
#include "components.h"
#include "entity_control.h"
#include "quad_tree.h"
#include "timer.h"

namespace AI {
    bool b_AI = true;

    //check for targets periodically
    //use the target x,y to move towards it
    //if target is in range, melee attack
    void Attack_Move(entt::registry &zone, entt::entity &entity_ID, entt::entity &target_ID, Component::Position &entityPosition, Component::Melee_Range &meleeRange, Component::Position &targetPosition, Component::Radius &targetRadius) {// maybe change to move and attack?
        if (zone.any_of<Component::Attacking>(entity_ID)) {
            return;
        }
        if (!zone.any_of<Component::Attacking>(entity_ID)) {
            //calcuate the point to move to that puts in range of melee attack on every few frames
            //if it is in range, run Melee_Attack()
            //else pass that point as an update to the move order
            if (Entity_Control::Target_In_Range(entityPosition, meleeRange.meleeRange, targetPosition, targetRadius)) {//check if center of attack rect is in the target
                auto &action = zone.get<Action_Component::Action>(entity_ID);
                if (action.state != Action_Component::attack && action.state != Action_Component::struck) {
                    Action_Component::Set_State(action, Action_Component::combatIdle);
                }
                zone.remove<Component::Mouse_Move>(entity_ID);
                zone.remove<Component::Moving>(entity_ID);
                zone.remove<Component::Pathing>(entity_ID);
                Entity_Control::Melee_Attack(zone, entity_ID, target_ID, targetPosition);
            } else {
                auto moveTo = A_Star::Move_To(zone, entity_ID, entityPosition, targetPosition);
                Entity_Control::Move_Order(zone, entity_ID, moveTo.x, moveTo.y);
            }
        }
        //else move to cursor
    }

    void Check_For_Targets(entt::registry &zone, int &state) {
        auto units = zone.view<Component::Sight_Range, Component::Alive, Component::Position, Component::Melee_Range>();
        //currently specifically looks for a player as a target using input component
        for (auto unit_ID: units) {
            auto &sightBox = units.get<Component::Sight_Range>(unit_ID).sightBox;
            auto &meleeRange = units.get<Component::Melee_Range>(unit_ID);
            auto &unitPosition = units.get<Component::Position>(unit_ID);

            std::vector<entt::entity> entityData = Quad_Tree::Get_Nearby_Entities(zone, unit_ID, sightBox, state);

            bool found = false;
            for (auto target: entityData) {
                if (zone.any_of<Component::Entity_Type>(target)) {
                    auto type = zone.get<Component::Entity_Type>(target);
                    if (type == Component::Entity_Type::unit) {
                        if (Social_Control::Check_Relationship(zone, unit_ID, target)) {
                            if (!zone.any_of<Component::Alive>(target)) {
                                Utilities::Log("target has no Component::Alive");
                                continue;
                            }
                            auto &alive = zone.get<Component::Alive>(target);
                            if (!alive.bIsAlive) {
                                Utilities::Log("target is dead");
                                continue;
                            }

                            auto &targetPosition = zone.get<Component::Position>(target);
                            auto &targetRadius = zone.get<Component::Radius>(target);
                            SDL_FPoint targetPoint = {targetPosition.x, targetPosition.y};

                            if (Utilities::bFPoint_FRectIntersect(targetPoint, sightBox)) {
				//TODO: cast a ray to check if there is line of sight, if not, return UNLESS they are in sound range AND the path is short OR the unit has a special ability to see through walls

                                found = true;
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
                    }
                }
            }
            //TODO: there is a bug, when you command a unit to move with pathing, it will pauses for a moment at each node

            if (!found && zone.any_of<Component::Location>(unit_ID)) {
                auto spawnPosition = zone.get<Component::Location>(unit_ID).current;
                if (!Movement_Functions::Check_If_Arrived(unitPosition, spawnPosition)) {
                    auto pathing = zone.emplace_or_replace<Component::Pathing>(unit_ID);
                    A_Star::Solve_AStar(unitPosition, spawnPosition, pathing.path);
                    //return to spawn location if he is not already there
                    if (pathing.path.empty()) {
                        Utilities::Log("In target Node, moving directly");
                        Entity_Control::Move_Order(zone, unit_ID, spawnPosition.x, spawnPosition.y);
                        break;
                    }
                    float y = (pathing.path[pathing.path.size() - 1].y * A_Star::nNodeSize) + (A_Star::nNodeSize / 2.0f);
                    float x = (pathing.path[pathing.path.size() - 1].x * A_Star::nNodeSize) + (A_Star::nNodeSize / 2.0f);
                    Entity_Control::Move_Order(zone, unit_ID, x, y);
                } else {
                    zone.remove<Component::Mouse_Move>(unit_ID);
                    zone.remove<Component::Moving>(unit_ID);
                    zone.remove<Component::Pathing>(unit_ID);
                }
            }
        }
    }

    void Update_Combat(entt::registry &zone) {
        auto units = zone.view<Component::In_Combat, Component::Attack_Speed>();
        for (auto unit_ID: units) {
            auto &attackSpeed = units.get<Component::Attack_Speed>(unit_ID);
            if (attackSpeed.counter > 0) {
                attackSpeed.counter -= Timer::timeStep;
            }
        }
    }

    void Update_Sight_Box(entt::registry &zone) {
        //could add Component::Moving so it only updates when the entity moves
        auto view = zone.view<Component::Sight_Range, Component::Position, Component::Alive>();
        for (auto entity: view) {
            auto &sight = view.get<Component::Sight_Range>(entity);
            auto &position = view.get<Component::Position>(entity);
            sight.sightBox = {position.x - (sight.range / 2.0f), position.y - (sight.range / 2.0f), sight.range, sight.range};
        }
    }

    void Update_Player_Target_Range(entt::registry &zone) {
        //could add Component::Moving so it only updates when the entity moves
        auto view = zone.view<Component::Target_Range, Component::Position, Component::Alive>();
        for (auto entity: view) {
            auto &sight = view.get<Component::Target_Range>(entity);
            auto &position = view.get<Component::Position>(entity);
            sight.rangeBox = {position.x - (sight.range / 2.0f), position.y - (sight.range / 2.0f), sight.range, sight.range};
        }
    }

    void Turn_On() {
        if (!b_AI) {
            b_AI = true;
        } else {
            b_AI = false;
        }
    }

    float time = 0;

    void Update_AI(entt::registry &zone, int &state) {
        if (b_AI) {
            time += Timer::timeStep;
            if (time >= 250.0f) {
                time = 0.0f;
                Update_Sight_Box(zone);
                Update_Player_Target_Range(zone);
                Check_For_Targets(zone, state);
            }
        }
        Update_Combat(zone);
    }
}// namespace AI
