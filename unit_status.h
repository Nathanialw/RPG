#pragma once
#include "entt/entt.hpp"
#include "components.h"
#include "combat_control.h"
#include "tooltips.h"
#include "graphics.h"
//#include "include/box2d/box2d.h"


namespace Unit_Status {
	void Update_Collided_Unit(entt::registry& zone) {
		auto view = zone.view<Component::Position, Component::Collided, Component::Entity_Type, Component::Alive>();
		for (auto entity : view) {
			auto& unitType = view.get<Component::Entity_Type>(entity);
			auto& position = view.get<Component::Position>(entity);
			auto& collide = view.get<Component::Collided>(entity);
			auto& alive = view.get<Component::Alive>(entity);
			position.x = collide.x;
			position.y = collide.y;
			if (unitType == Component::Entity_Type::spell || unitType == Component::Entity_Type::melee) {
				alive.bIsAlive = false; //destroy spell on contact
			}
			if (unitType == Component::Entity_Type::unit) {
				for (int i = 0; i < collide.InteractionList.size(); i++) {
					if (collide.InteractionList[i].type == Component::Entity_Type::spell || collide.InteractionList[i].type == Component::Entity_Type::melee) {
                        Component::Damage& damageRange = zone.get<Component::Damage>(collide.InteractionList[i].entity);
						int damage = Combat_Control::Calculate_Damage(damageRange);

						auto &caster_ID = zone.get<Component::Caster_ID>(collide.InteractionList[i].entity).caster_ID;
						Damage_Text::Add_To_Scrolling_Damage(zone, caster_ID, entity, damage);
						//std::cout << damage << std::endl;

						auto& struck = zone.get_or_emplace<Component::Struck>(entity);
						struck.struck += damage;
					}
				}
			}
			zone.remove<Component::Collided>(entity);
		}
	}

	void Update_Health(entt::registry& zone) {
		auto view2 = zone.view<Component::Health, Component::Struck>();
		for (auto entity : view2) {
			auto& damage = view2.get<Component::Struck>(entity).struck;
			auto& health = view2.get<Component::Health>(entity).currentHealth;
			//std::cout << "health = " << health << std::endl;
			health -= damage;
			damage -= damage;
			//if the soldier is in the assignment vector it will be set as dead if it dies
			if (health <= 0) {
				if (zone.any_of<Component::Assigned_To_Formation>(entity)) {
					auto& assignment = zone.get<Component::Assigned_To_Formation>(entity);
					auto& squad = zone.get<Test::Soldiers_Assigned_List>(assignment.iUnit_Assigned_To);
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
//			//if the soldier is in the assignment vector it will be set as dead if it dies
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

	void isDead(entt::registry& zone) {
        auto view = zone.view<Component::Sprite_Sheet_Info, Component::Action, Component::Health, Component::Position, Component::Radius, Component::Sprite_Offset, Component::Body, Component::In_Object_Tree>(entt::exclude<Component::Spell>);
        for (auto entity: view) {
            auto &health = view.get<Component::Health>(entity);
            if (health.currentHealth <= 0) {
                view.get<Component::Action>(entity).state = Component::dead;
                view.get<Component::Sprite_Sheet_Info>(entity).currentFrame = 0;
                auto &position = view.get<Component::Position>(entity);
                auto &radius = view.get<Component::Radius>(entity).fRadius;
                auto &offset = view.get<Component::Sprite_Offset>(entity);
                auto &body = view.get<Component::Body>(entity).body;
                auto &inTree = view.get<Component::In_Object_Tree>(entity).inTree;
                Items::Create_And_Drop_Item(position);
                ///sets the sprite to render so that it is always rendered behind living sprites
                position.x -= offset.x;
                position.y -= offset.y;
                offset.x = 0.0f;
                offset.y = 0.0f;
                Collision::world->DestroyBody(body);
                World::zone.remove<Component::Body>(entity);
                ///set to remove from quad tree on update
                SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(radius, position.x, position.y);
                zone.emplace<Component::Remove_From_Object_Tree>(entity, rect);
                zone.get<Component::Alive>(entity).bIsAlive = false;
                zone.remove<Component::Commandable>(entity);
                zone.remove<Component::Selected>(entity);
                zone.remove<Component::Moving>(entity);
                zone.remove<Component::Mouse_Move>(entity);
                zone.remove<Component::Velocity>(entity);
                zone.remove<Component::Spellbook>(entity);
                zone.remove<Component::Mass>(entity);
                zone.remove<Component::Sight_Range>(entity);
                zone.remove<Component::Health>(entity);
                zone.remove<Component::Radius>(entity);
                if (zone.any_of<Component::Assigned_To_Formation>(entity)) {
                    auto &soldier = zone.get<Component::Assigned_To_Formation>(entity);
                    auto &soldier_list = zone.get<Test::Soldiers_Assigned_List>(soldier.iUnit_Assigned_To);
                    soldier_list.unitData[soldier.iIndex].bAlive = false;
                    zone.remove<Component::Assigned_To_Formation>(entity);
                }
            }
        }
    }

	void Update_Unit_Status(entt::registry &zone) {
		Update_Collided_Unit(zone);
		Update_Health(zone);
		isDead(zone);
	}
}
