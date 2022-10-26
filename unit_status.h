#pragma once
#include "entt.hpp"
#include "components.h"
#include "combat_control.h"
#include "tooltips.h"
#include "graphics.h"
//#include "include/box2d/box2d.h"


namespace Unit_Status {



	void Update_Collided_Unit(entt::registry& zone) {
		auto view = zone.view<Position, Collided, Entity_Type, Alive>();

		for (auto entity : view) {
			auto& unitType = view.get<Entity_Type>(entity);
			auto& x = view.get<Position>(entity);
			auto& y = view.get<Position>(entity);
			auto& collide = view.get<Collided>(entity);
			auto& alive = view.get<Alive>(entity);
			//std::cout << collide.x << std::endl;
			//std::cout << collide.y << std::endl;


			x.x = collide.x;
			y.y = collide.y;

			if (unitType == Component::Entity_Type::spell || unitType == Entity_Type::melee) {
				alive.bIsAlive = false; //destroy spell on contact
			}

			if (unitType == Component::Entity_Type::unit) {
				for (int i = 0; i < collide.InteractionList.size(); i++) {
					if (collide.InteractionList[i].type == Entity_Type::spell || collide.InteractionList[i].type == Entity_Type::melee) {
 						Damage& damageRange = zone.get<Damage>(collide.InteractionList[i].entity);
						int damage = Combat_Control::Calculate_Damage(damageRange);

						auto &caster_ID = zone.get<Caster_ID>(collide.InteractionList[i].entity).caster_ID;
						Damage_Text::Add_To_Scrolling_Damage(zone, caster_ID, entity, damage);
						//std::cout << damage << std::endl;

						auto& struck = zone.get_or_emplace<Struck>(entity);
						struck.struck += damage;
					}
				}
			}

			zone.remove<Collided>(entity);
		}
	}

	void Update_Health(entt::registry& zone) {
		auto view2 = zone.view<Component::Health, Component::Struck, Component::Actions>();

		for (auto entity : view2) {
			auto& damage = view2.get<Component::Struck>(entity).struck;
			auto& health = view2.get<Component::Health>(entity).currentHealth;
			auto& action = view2.get<Component::Actions>(entity).action;

			action = Component::struck;

			//std::cout << "health = " << health << std::endl;
			health -= damage;
			zone.remove<Component::Struck>(entity);

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
		auto view = zone.view<Component::Health, Component::Struck, Component::Action_State>();

		for (auto entity : view) {
			auto& damage = view.get<Component::Struck>(entity).struck;
			auto& health = view.get<Component::Health>(entity).currentHealth;
			auto& action = view.get<Component::Action_State>(entity);

			action = Component::struck;

			//std::cout << "health = " << health << std::endl;
			health -= damage;
			zone.remove<Component::Struck>(entity);

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
	}

	void isDead(entt::registry& zone) {
		auto view = zone.view<Actions, Health, Position, Radius, Sprite_Offset, Body, In_Object_Tree>(entt::exclude<Spell>);
		for (auto entity : view) {
			auto& health = view.get<Health>(entity);
			if (health.currentHealth <= 0) {
				view.get<Actions>(entity).action = dead;
				view.get<Actions>(entity).frameCount[view.get<Actions>(entity).action].currentFrame = 0;
				auto& position = view.get<Position>(entity);
				auto& radius = view.get<Radius>(entity).fRadius;
				auto& offset = view.get<Sprite_Offset>(entity).offset;
				auto& body = view.get<Body>(entity).body;
				auto& inTree = view.get<In_Object_Tree>(entity).inTree;

				Items::Create_And_Drop_Item(position);

				//sets the sprite to render so that it is always rendered behind living sprites
				position.x -= offset.x;
				position.y -= offset.y;
				offset.x = 0.0f;
				offset.y = 0.0f;

				Collision::world->DestroyBody(body);
				World::zone.remove<Body>(entity);
				//set to remove from quad tree on update
				SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(radius, position.x, position.y);
				zone.emplace<Remove_From_Object_Tree>(entity, rect);


				zone.get<Alive>(entity).bIsAlive = false;
				zone.remove<Commandable>(entity);
				zone.remove<Selected>(entity);
				zone.remove<Moving>(entity);
				zone.remove<Mouse_Move>(entity);
				zone.remove<Velocity>(entity);
				zone.remove<Spellbook>(entity);
				zone.remove<Mass>(entity);
				zone.remove<Sight_Range>(entity);
				zone.remove<Health>(entity);
				zone.remove<Radius>(entity);

				if (zone.any_of<Assigned_To_Formation>(entity)) {
					auto &soldier = zone.get<Assigned_To_Formation>(entity);
					auto &soldier_list = zone.get<Test::Soldiers_Assigned_List>(soldier.iUnit_Assigned_To);
					soldier_list.unitData[soldier.iIndex].bAlive = false;
					zone.remove<Assigned_To_Formation>(entity);
				}
			}
		}

		auto view2 = zone.view<Action_State, Health, Position, Radius, Sprite_Offset, Body, In_Object_Tree, Sprite_Vector>(entt::exclude<Spell>);
		for (auto entity : view2) {
			auto& health = view2.get<Health>(entity);
			if (health.currentHealth <= 0) {
				auto& state = view2.get<Action_State>(entity);
				state = Action_State::dead;
				int& currentFrame = view2.get<Sprite_Vector>(entity).currentFrame;
				currentFrame = 0;

				auto& position = view2.get<Position>(entity);
				auto& sprite = view2.get<Sprite_Vector>(entity);
				auto& radius = view2.get<Radius>(entity).fRadius;
				auto& offset = view2.get<Sprite_Offset>(entity).offset;
				auto& body = view2.get<Body>(entity).body;
				auto& inTree = view2.get<In_Object_Tree>(entity).inTree;

				sprite.currentFrame = 0;


				Items::Create_And_Drop_Item(position);

				//sets the sprite to render so that it is always rendered behind living sprites
				position.x -= offset.x;
				position.y -= offset.y;
				offset.x = 0.0f;
				offset.y = 0.0f;

				Collision::world->DestroyBody(body);
				World::zone.remove<Body>(entity);
				//set to remove from quad tree on update
				SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(radius, position.x, position.y);
				zone.emplace<Remove_From_Object_Tree>(entity, rect);


				zone.get<Alive>(entity).bIsAlive = false;
				zone.remove<Commandable>(entity);
				zone.remove<Selected>(entity);
				zone.remove<Moving>(entity);
				zone.remove<Mouse_Move>(entity);
				zone.remove<Velocity>(entity);
				zone.remove<Spellbook>(entity);
				zone.remove<Mass>(entity);
				zone.remove<Sight_Range>(entity);
				zone.remove<Health>(entity);
				zone.remove<Radius>(entity);

			}
		}
	}


	void Update_Unit_Status(entt::registry &zone) {
		Update_Collided_Unit(zone);
		Update_Health(zone);
		isDead(zone);
	}

}
