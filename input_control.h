#pragma once
#include "entt.hpp"
#include "components.h"
#include "mouse_control.h"
#include "dynamic_quad_tree.h"
#include "player_control.h"
#include "ui.h"

namespace Input_Control {


	//need to make a move to item then pick it up routine for these components
	void Pick_Up_Item_Order(entt::registry& zone, entt::entity& entity, entt::entity& Item_ID, float& x, float& y) {
		zone.emplace_or_replace<Component::Item_Pickup>(entity, Item_ID, x, y);
		zone.emplace_or_replace<Component::Moving>(entity);
	}

    bool testasd(entt::entity &player, entt::entity &target) {
        if (player == target) {
            return true;
        }
        return false;
    };

    bool Move_To_Item_From_Name(entt::registry& zone, entt::entity& player_ID, Position& playerPosition, entt::entity item_ID){

        auto& radius = zone.get<Radius>(player_ID).fRadius;
        SDL_FRect unitRect = Utilities::Get_FRect_From_Point_Radius(radius, playerPosition.x, playerPosition.y);

        Component::Entity_Type& type = zone.get<Component::Entity_Type>(item_ID);
        Position& targetPosition = zone.get<Position>(item_ID);
        Radius& targetRadius = zone.get<Radius>(item_ID);
        SDL_FRect itemRect = Utilities::Get_FRect_From_Point_Radius(targetRadius.fRadius, targetPosition.x, targetPosition.y);

        if (Utilities::bFRect_Intersect(unitRect, itemRect)) {
            //pick up Item
            Item_Pickup itemData = { item_ID, targetPosition.x, targetPosition.y, targetRadius.fRadius};
            UI::Pick_Up_Item_To_Mouse_Or_Bag(zone, itemData, Mouse::itemCurrentlyHeld);
            zone.remove<Moving>(player_ID);
            zone.remove<Item_Pickup>(player_ID);
            return true;
        }
        else {
            //Move to Item then pick it up
            Pick_Up_Item_Order(zone, player_ID, item_ID, targetPosition.x, targetPosition.y);
            return true;
        }
    }

    int i = 0;

	bool Check_For_Mouse_Target(entt::registry& zone, bool showGroundItems, entt::entity& player_ID, Position& playerPosition, Melee_Range& meleeRange) {
        i++;
		if (World::zone.any_of<Attacking>(player_ID) == true) {
			return true;
		}

        if (showGroundItems) {
            auto view = zone.view<Ground_Item, Position, Rarity, Name, Renderable>();
            for (auto item_ID: view) {
                //get the item ID the mouse is over nad plug it into the move to pick up function
                auto Item_Name_Box = view.get<Ground_Item>(item_ID).ground_name;
                //only if the mouse intersects with the item box
                if (Mouse::bRect_inside_Cursor(Item_Name_Box)) {
                    if (Input_Control::Move_To_Item_From_Name(zone, player_ID, playerPosition, item_ID)) {
                        return true;
                    };
                }
            }
        }

        SDL_FRect mouseRect = Utilities::Get_FRect_From_Point_Radius(Mouse::cursorSize, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
        Dynamic_Quad_Tree::Entity_Data targetData = Dynamic_Quad_Tree::Entity_vs_Mouse_Collision(zone, mouseRect);

		if (targetData.b == true) {
			zone.remove<Item_Pickup>(player_ID);
			zone.remove<Moving>(player_ID);

			Component::Entity_Type& type = zone.get<Component::Entity_Type>(targetData.entity_ID);
			Position& targetPosition = zone.get<Position>(targetData.entity_ID);
			Radius& targetRadius = zone.get<Radius>(targetData.entity_ID);

			switch (type) {
                case Component::Entity_Type::unit:

                if (player_ID != targetData.entity_ID) {
                    if (AI::Player_In_Melee_Range(zone, playerPosition, meleeRange, targetPosition, targetRadius)) {
                        zone.remove<Moving>(player_ID);
                        AI::Melee_Attack(zone, player_ID, targetData.entity_ID, targetPosition);
                      //  std::cout << "moving to target" << std::endl;
                        return true;
                    } else {
                        Player_Control::Attack_Order(zone, player_ID, targetData.entity_ID, targetPosition,targetRadius);
                      //  std::cout << "attacking" << std::endl;
                        return true;
                    }
                }
                else {
                  //  std::cout << "no target, 1 is targetting player: " << testasd(player_ID, targetData.entity_ID) << std::endl;
                }

                case Component::Entity_Type::item:

                if (!showGroundItems) {

                    auto &radius = zone.get<Radius>(player_ID).fRadius;
                    SDL_FRect unitRect = Utilities::Get_FRect_From_Point_Radius(radius, playerPosition.x,playerPosition.y);
                    SDL_FRect itemRect = Utilities::Get_FRect_From_Point_Radius(targetRadius.fRadius,targetPosition.x, targetPosition.y);

                    //if player is next to the item
                    if (Utilities::bFRect_Intersect(unitRect, itemRect)) {
                        //pick up Item
                        Item_Pickup itemData = {targetData.entity_ID, targetPosition.x, targetPosition.y,targetRadius.fRadius};
                        UI::Pick_Up_Item_To_Mouse_Or_Bag(zone, itemData, Mouse::itemCurrentlyHeld);
                        zone.remove<Moving>(player_ID);
                        return true;
                    } else {
                        //Move to Item then pick it up
                        Pick_Up_Item_Order(zone, player_ID, targetData.entity_ID, targetPosition.x,targetPosition.y);
                        return true;
                    }
                }
			}
		}
		return false;
	}
}