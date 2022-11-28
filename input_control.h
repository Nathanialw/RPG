#pragma once
#include "entt/entt.hpp"
#include "components.h"
#include "mouse_control.h"
#include "dynamic_quad_tree.h"
#include "player_control.h"
#include "entity_control.h"
#include "social_control.h"

namespace Input_Control {
	    /// cand probaby merge the routine with Component::Pickup_Item and the normal mouse move, they are almost the same
	void Pick_Up_Item_Order(entt::registry& zone, entt::entity& entity, entt::entity& Item_ID, float& x, float& y) {
		zone.emplace_or_replace<Component::Pickup_Item>(entity, Item_ID, x, y);
		zone.emplace_or_replace<Component::Moving>(entity);
        zone.remove<Component::Mouse_Move>(entity);
	}

    bool Move_To_Item_From_Name(entt::registry& zone, entt::entity& player_ID, Component::Position& playerPosition, entt::entity item_ID){
        auto& radius = zone.get<Component::Radius>(player_ID).fRadius;
        SDL_FRect unitRect = Utilities::Get_FRect_From_Point_Radius(radius, playerPosition.x, playerPosition.y);

        Component::Entity_Type& type = zone.get<Component::Entity_Type>(item_ID);
        Component::Position& targetPosition = zone.get<Component::Position>(item_ID);
        Component::Radius& targetRadius = zone.get<Component::Radius>(item_ID);

        SDL_FRect itemRect = Utilities::Get_FRect_From_Point_Radius(targetRadius.fRadius, targetPosition.x, targetPosition.y);

        if (Utilities::bFRect_Intersect(unitRect, itemRect)) {
                ///pick up Item
            Component::Pickup_Item itemData = {item_ID, targetPosition.x, targetPosition.y, targetRadius.fRadius};
            UI::Pick_Up_Item_To_Mouse_Or_Bag(zone, itemData, Mouse::itemCurrentlyHeld);

            auto &action = zone.get<Component::Action>(player_ID);
            action.state = Component::idle;

            zone.remove<Component::Moving>(player_ID);
            zone.remove<Component::Pickup_Item>(player_ID);
            return true;
        }
        else {
                ///Move to Item then pick it up
            Pick_Up_Item_Order(zone, player_ID, item_ID, targetPosition.x, targetPosition.y);
            return true;
        }
    }

    bool Check_For_Mouse_Target(entt::registry& zone, bool showGroundItems, entt::entity& player_ID, Component::Position& playerPosition, Component::Melee_Range& meleeRange) {
        if (World::zone.any_of<Component::Attacking>(player_ID) == true) {
			return true;
		}

        zone.remove<Component::Pickup_Item>(player_ID);
        zone.remove<Component::Moving>(player_ID);
        zone.remove<Player_Component::Attack_Move>(player_ID);

        if (showGroundItems) {
            auto view = zone.view<Ground_Item, Component::Position, Rarity, Name, Component::Renderable>();
            for (auto item_ID: view) {
                    ///get the item ID the mouse is over nad plug it into the move to pick up function
                auto Item_Name_Box = view.get<Ground_Item>(item_ID).ground_name;
                    ///only if the mouse intersects with the item box
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
                ///reset target data
			Component::Entity_Type& type = zone.get<Component::Entity_Type>(targetData.entity_ID);
            Component::Position& targetPosition = zone.get<Component::Position>(targetData.entity_ID);
            Component::Radius& targetRadius = zone.get<Component::Radius>(targetData.entity_ID);
			switch (type) {
                case Component::Entity_Type::unit:
                if (player_ID != targetData.entity_ID) {
                    Player_Control::Attack_Order(zone, player_ID, targetData.entity_ID, targetPosition, targetRadius);
                    return true;
                }
                else {
                  //  std::cout << "no target, 1 is targetting player: " << testasd(player_ID, targetData.entity_ID) << std::endl;
                }

                case Component::Entity_Type::item:
                if (!showGroundItems) {
                    auto &radius = zone.get<Component::Radius>(player_ID).fRadius;
                    SDL_FRect unitRect = Utilities::Get_FRect_From_Point_Radius(radius, playerPosition.x,playerPosition.y);
                    SDL_FRect itemRect = Utilities::Get_FRect_From_Point_Radius(targetRadius.fRadius,targetPosition.x, targetPosition.y);
                        ///if player is next to the item
                    if (Utilities::bFRect_Intersect(unitRect, itemRect)) {
                            ///pick up Item
                        Component::Pickup_Item itemData = {targetData.entity_ID, targetPosition.x, targetPosition.y, targetRadius.fRadius};
                        UI::Pick_Up_Item_To_Mouse_Or_Bag(zone, itemData, Mouse::itemCurrentlyHeld);
                            ///stop movement
                        auto &action = zone.get<Component::Action>(player_ID);
                        action.state = Component::idle;
                        zone.remove<Component::Moving>(player_ID);
                        return true;
                    } else {
                            ///Move to Item then pick it up
                        Pick_Up_Item_Order(zone, player_ID, targetData.entity_ID, targetPosition.x,targetPosition.y);
                        return true;
                    }
                }
			}
		}
		return false;
	}
}