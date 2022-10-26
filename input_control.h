#pragma once
#include "entt.hpp"
#include "components.h"
#include "mouse_control.h"
#include "dynamic_quad_tree.h"
#include "player_control.h"
#include "ui.h"

namespace Input_Control {

	struct Entity_Data {
		bool b = false;
		entt::entity entity_ID;
	};

	Entity_Data Entity_vs_Mouse_Collision(entt::registry& zone, Dynamic_Quad_Tree::DynamicQuadTreeContainer<Dynamic_Quad_Tree::someObjectWithArea> &treeObjects) {
		
		int i = 0;
		SDL_FRect mouseRect = Utilities::Get_FRect_From_Point_Radius(Mouse::cursorSize, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);

		for (const auto& object : treeObjects.search(mouseRect)) {
			//prevents plater from reutrning themself from the quadtree
			//should probably make the player entity ID a constant saved somewhere, it may not always be 1
			if ((int)object->item.entity_ID == 1) { 
				continue; 
			};
			return { true, object->item.entity_ID };
		}
		return { false };
	}

	
	//need to make a move to item then pick it up routine for these components
	void Pick_Up_Item_Order(entt::registry& zone, entt::entity& entity, entt::entity& Item_ID, float& x, float& y) {
		zone.emplace_or_replace<Component::Item_Pickup>(entity, Item_ID, x, y);
		zone.emplace_or_replace<Component::Moving>(entity);
	}


	bool Check_For_Mouse_Target(entt::registry& zone, entt::entity& player_ID, Position& playerPosition, Melee_Range& meleeRange) {

		if (World::zone.any_of<Attacking>(player_ID) == true) {
			return true;
		}

		Entity_Data targetData = Entity_vs_Mouse_Collision(zone, Dynamic_Quad_Tree::treeObjects);
		if (targetData.b == true) {
			zone.remove<Item_Pickup>(player_ID);
			zone.remove<Moving>(player_ID);
			
			Component::Entity_Type& type = zone.get<Component::Entity_Type>(targetData.entity_ID);
			Position& targetPosition = zone.get<Position>(targetData.entity_ID);
			Radius& targetRadius = zone.get<Radius>(targetData.entity_ID);

			switch (type) {
			case Component::Entity_Type::unit:

				if (AI::Player_In_Melee_Range(zone, playerPosition, meleeRange, targetPosition, targetRadius)) {
					zone.remove<Moving>(player_ID);
					AI::Melee_Attack(zone, player_ID, targetData.entity_ID, targetPosition);
					return true;
				}
				else {
					Player_Control::Attack_Order(zone, player_ID, targetData.entity_ID, targetPosition, targetRadius);
					return true;
				}

			case Component::Entity_Type::item:
				auto& radius = zone.get<Radius>(player_ID).fRadius;
				SDL_FRect unitRect = Utilities::Get_FRect_From_Point_Radius(radius, playerPosition.x, playerPosition.y);
				SDL_FRect itemRect = Utilities::Get_FRect_From_Point_Radius(targetRadius.fRadius, targetPosition.x, targetPosition.y);

				//if player is next to the item
				if (Utilities::bFRect_Intersect(itemRect, unitRect)) {
					//pick up Item
					Item_Pickup itemData = { targetData.entity_ID, targetPosition.x, targetPosition.y, targetRadius.fRadius};
					UI::Pick_Up_Item_To_Mouse_Or_Bag(zone, itemData, Mouse::itemCurrentlyHeld);
					zone.remove<Moving>(player_ID);
					return true;
				}
				else {
					//Move to Item then pick it up
					Pick_Up_Item_Order(zone, player_ID, targetData.entity_ID, targetPosition.x, targetPosition.y);
					return true;
				}
			}
		}
		return false;
	}
	
}