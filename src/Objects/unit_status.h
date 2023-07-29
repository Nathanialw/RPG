#pragma once

#include "entt/entt.hpp"
#include "components.h"
#include "combat_control.h"
#include "items.h"
#include "tooltips.h"
#include "graphics.h"
#include "death_control.h"
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
        alive.bIsAlive = false; //destroy spell on contact
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

  void Update_Health(entt::registry &zone) {
    auto view2 = zone.view<Component::Position, Component::Health, Component::Struck>();
    for (auto entity: view2) {
      auto &struck = view2.get<Component::Struck>(entity);
      auto &position = view2.get<Component::Position>(entity);
      auto &health = view2.get<Component::Health>(entity).currentHealth;
      //std::cout << "health = " << health << std::endl;
      health -= struck.struck;
      struck.struck -= struck.struck;

      if (struck.critical) {
        //reset
        struck.critical = false;
        // spawn blood
        std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
        Collision::aabb aabb;
        Component::Line_Segment line;
        tmx::Vector2<float> offset = {0.0f, 0.0f};

        int splatterIndex = Utilities::Get_Random_Number(1, Game_Objects_Lists::bloodSplatterVec.size() - 1);
        Create_Entities::PVG_Building(zone, position.x, position.y, position.x, position.y, Game_Objects_Lists::bloodSplatterVec[splatterIndex], splatterIndex, aabb, pointVecs, line, offset);
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

  void Update_Unit_Status(entt::registry &zone) {
    Items::Item_Collision(zone);
    Update_Collided_Unit(zone);
    Update_Health(zone);
    Death_Control::isDead(zone);
    Death_Control::Dead_Entity_Routine(zone);
  }
}
