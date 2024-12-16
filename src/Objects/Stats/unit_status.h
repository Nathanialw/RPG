#pragma once

#include "Items/items.h"
#include "Objects/Combat/combat_control.h"
#include "Objects/Combat/death_control.h"
#include "blood.h"
#include "components.h"
#include "entt/entt.hpp"
#include "graphics.h"
#include "tooltips.h"
#include "utilities.h"

namespace Unit_Status {
  void Update_Collided_Unit(entt::registry &zone) {
    auto view = zone.view<Component::Position, Component::Collided, Component::Entity_Type, Component::Alive>();
    for (auto entity: view) {
      auto &unitType = view.get<Component::Entity_Type>(entity);
      auto &position = view.get<Component::Position>(entity);
      auto &collide = view.get<Component::Collided>(entity);
      auto &alive = view.get<Component::Alive>(entity);
      position.x = collide.x;
      position.y = collide.y;
      if (unitType == Component::Entity_Type::spell) {
        alive.bIsAlive = false;//destroy spell on contact
      }
      if (unitType == Component::Entity_Type::unit) {
        for (int i = 0; i < collide.InteractionList.size(); i++) {
          if (collide.InteractionList[i].type == Component::Entity_Type::spell) {
            // Component::Damage& damageRange = zone.get<Component::Damage>(collide.InteractionList[i].entity);
            // int damage = Combat_Control::Calculate_Damage(damageRange);

            // auto &caster_ID = zone.get<Component::Caster_ID>(collide.InteractionList[i].entity).caster_ID;
            // Damage_Text::Add_To_Scrolling_Damage(zone, caster_ID, entity, damage, false, damage);

            // auto& struck = zone.get_or_emplace<Component::Struck>(entity);
            // if (damageRange.critical) {
            //   struck.critical = true;
            // }
            // struck.struck += damage;
          }
        }
      }
      zone.remove<Component::Collided>(entity);
    }
  }

  void Update_Health(entt::registry &zone, int &state) {
    auto view2 = zone.view<Component::Position, Component::Health, Component::Struck>();
    for (auto entity: view2) {
      auto &struck = view2.get<Component::Struck>(entity);
      auto &position = view2.get<Component::Position>(entity);
      auto &health = view2.get<Component::Health>(entity).currentHealth;
      //std::cout << "health = " << health << std::endl;
      health -= struck.struck;
      struck.struck -= struck.struck;

      if (struck.critical) {
        //reset crit state
        struck.critical = false;
        Blood::Splatter(zone, state, position);
      }

      //if the soldier is in the assignment vector it will be set as dying if it dies
      if (health <= 0) {
        if (zone.any_of<Component::Assigned_To_Formation>(entity)) {
          auto &assignment = zone.get<Component::Assigned_To_Formation>(entity);
          auto &squad = zone.get<Test::Soldiers_Assigned_List>(assignment.iUnit_Assigned_To);
          squad.unitData.at(assignment.iIndex).bAlive = false;
        }
      }
      if (zone.any_of<Component::Input>(entity)) {
        //Items::statData[Item_Component::Stat::health] -= health;
      }
    }
    //		auto view = zone.view<Component::Health, Component::Struck, Component::Action_State>();
    //		for (auto entity : view) {
    //			auto& damage = view.get<Component::Struck>(entity).struck;
    //			auto& health = view.get<Component::Health>(entity).currentHealth;
    //			auto& state = view.get<Component::Action_State>(entity);
    //			state = Component::struck;
    //			//std::cout << "health = " << health << std::endl;
    //			health -= damage;
    //	        damage -= damage;
    //			//if the soldier is in the assignment vector it will be set as dying if it dies
    //			if (health <= 0) {
    //				if (zone.any_of<Component::Assigned_To_Formation>(entity)) {
    //					auto& assignment = zone.get<Component::Assigned_To_Formation>(entity);
    //					auto& squad = zone.get<Test::Soldiers_Assigned_List>(assignment.iUnit_Assigned_To);
    //					squad.unitData.at(assignment.iIndex).bAlive = false;
    //				}
    //			}
    //			if (zone.any_of<Component::Input>(entity)) {
    //				//Items::statData[Item_Component::Stat::health] -= health;
    //			}
    //		}
  }

  void Update_Unit_Status(entt::registry &zone, int &state) {
    Items::Item_Collision(zone);
    Update_Collided_Unit(zone);
    Update_Health(zone, state);
    Death_Control::isDead(zone, state);
    Death_Control::Dead_Entity_Routine(zone, state);
    XP::Update(zone, state);
  }
}// namespace Unit_Status
