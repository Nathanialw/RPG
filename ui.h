#pragma once
#include "components.h"
#include "camera.h"
#include "menu.h"
#include "spellbook.h"

namespace UI {
	
	SDL_Rect charui = { 0, 0, 554, 1080 };
	SDL_FRect defaultScreenPosition = { 100.0f, 100.0f, 554.0f, 1080.0f };
	SDL_FRect Character_UI = {};
	bool bToggleCharacterUI = false;

	namespace Bag_UI {
		namespace {
			entt::entity emptyBagSlot;
			DataTypes::f2d bagoffset = { 32.0f, 544.0f };
			DataTypes::f2d numOfSlots = { 8.0f, 4.0f };
			int iTotalSlots = (int)numOfSlots.x * (int)numOfSlots.y;
			float iBagSlotPixelSize = 64.0f;
			SDL_FRect bagRect = { defaultScreenPosition.x + bagoffset.x, defaultScreenPosition.y + bagoffset.y, numOfSlots.x * iBagSlotPixelSize, numOfSlots.y * iBagSlotPixelSize };
			SDL_FRect screenBag = { };
			std::vector<entt::entity>UI_bagSlots(iTotalSlots);
		}

		void Place_Item_In_Bag(entt::registry& zone, entt::entity& item, bool& mouseHasItem, int& slotNum) {
			UI_bagSlots.at(slotNum) = item;
			mouseHasItem = false;
			zone.remove<Component::On_Mouse>(item);
			zone.emplace_or_replace<Component::Inventory>(item);
		}

		void Remove_Item_From_Bag(entt::registry& zone, entt::entity& item, bool& mouseHasItem, int& slotNum) {
			item = UI_bagSlots.at(slotNum);
			UI_bagSlots.at(slotNum) = emptyBagSlot;
			zone.emplace<Component::On_Mouse>(item);
			zone.remove<Component::Inventory>(item);
			mouseHasItem = true;
		}

		void Bag_Item_And_Swap_With_Mouse(entt::registry& zone, entt::entity& item, int& slotNum) {
			entt::entity itemInSlot = UI_bagSlots[slotNum];
			UI_bagSlots[slotNum] = item;
			zone.remove<Component::On_Mouse>(item);
			zone.emplace<Component::Inventory>(item);
			item = itemInSlot;
			zone.emplace<Component::On_Mouse>(item);
			zone.remove<Component::Inventory>(item);
		}

		void Create_Bag_UI(entt::registry& zone) {
			emptyBagSlot = Graphics::Create_Icon_Entity(zone, Graphics::emptyBagIcon, Graphics::bagSlotBorder);
			for (int i = 0; i < (iTotalSlots); i++) {
				UI_bagSlots.at(i) = emptyBagSlot;
			}
		}

		void Toggle_Bag() {
			if (!bToggleCharacterUI) {
				bToggleCharacterUI = true;
			}
			else {
				bToggleCharacterUI = false;
			}
		}

		//check if the Mouse point is in the rect and which one
		int Get_Bag_Slot(entt::registry& zone, SDL_FPoint& screenCursor, Component::Camera& camera) {
			SDL_FRect slotRect = {};
			slotRect.w = iBagSlotPixelSize;
			slotRect.h = iBagSlotPixelSize;
			int i;
			int j;
			int slot = 0;
			for (i = 0; i < numOfSlots.x; i++) {
				slotRect.x = (i * iBagSlotPixelSize) + bagRect.x;
				for (j = 0; j < numOfSlots.y; j++) {
					slotRect.y = (j * iBagSlotPixelSize) + bagRect.y;
					SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(zone, slotRect, camera);
					if (Utilities::bPoint_RectIntersect(screenCursor, scaledSlot)) {
						return slot;
					}
					if (j < (numOfSlots.y - 1)) {
						slot++;
					}
				}
				SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(zone, slotRect, camera);
				if (Utilities::bPoint_RectIntersect(screenCursor, scaledSlot)) {
					return slot;
				}
				if (i < (numOfSlots.x - 1)) {
					slot++;
				}
			}
			return 0;
		}

		//check if the Mouse point is in the rect and which one
		void Render_Bag_Slot(entt::registry& zone, SDL_Renderer* renderer, Component::Camera &camera) {
			SDL_FRect slotRect = {};
			slotRect.w = iBagSlotPixelSize;
			slotRect.h = iBagSlotPixelSize;
			float i;
			float j;
			int slot = 0;
			for (i = 0; i < numOfSlots.x; i++) {
				slotRect.x = (i * iBagSlotPixelSize) + bagRect.x;
				for (j = 0; j < numOfSlots.y; j++) {
					slotRect.y = (j * iBagSlotPixelSize) + bagRect.y;
					auto& icon = zone.get<Component::Icon>(UI_bagSlots.at(slot));
					/*could be injected instead if it was a class object method*/SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(zone, slotRect, camera);
					SDL_SetTextureAlphaMod(icon.pTexture, 255);
					SDL_SetTextureAlphaMod(icon.pIconRarityBorder, 255);
					SDL_SetTextureAlphaMod(icon.pIconBorder, 255);
					SDL_RenderCopyF(renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
					SDL_RenderCopyF(renderer, icon.pIconRarityBorder, &icon.clipIcon, &scaledSlot);
					SDL_RenderCopyF(renderer, icon.pIconBorder, &icon.clipIcon, &scaledSlot);
					if (j < (numOfSlots.y - 1)) {
						slot++;
					}
				}
				auto& icon = zone.get<Component::Icon>(UI_bagSlots.at(slot));
				/*could be injected instead if it was a class object method*/SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(zone, slotRect, camera);
				SDL_SetTextureAlphaMod(icon.pTexture, 255);
				SDL_SetTextureAlphaMod(icon.pIconRarityBorder, 255);
				SDL_SetTextureAlphaMod(icon.pIconBorder, 255);
				SDL_RenderCopyF(renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
				SDL_RenderCopyF(renderer, icon.pIconRarityBorder, &icon.clipIcon, &scaledSlot);
				SDL_RenderCopyF(renderer, icon.pIconBorder, &icon.clipIcon, &scaledSlot);
				if (i < (numOfSlots.x - 1)) {
					slot++;
				}
			}
		}

		bool Is_Cursor_Inside_Bag_Area(entt::registry& zone, Component::Camera& camera, SDL_FPoint &screenCursor) {
			return Utilities::bPoint_RectIntersect(screenCursor, screenBag);
		}

		void Interact_With_Bag(entt::registry& zone, entt::entity& item, SDL_FPoint& mousePoint, bool& mouseHasItem, Component::Camera& camera) {
			if (Mouse::bRect_inside_Cursor(Character_UI)) {
				int slotNum = Get_Bag_Slot(zone, mousePoint, camera);

				if (Is_Cursor_Inside_Bag_Area(zone, camera, mousePoint)) {
					if (mouseHasItem) {
						if (UI_bagSlots[slotNum] == emptyBagSlot) {
							Place_Item_In_Bag(zone, item, mouseHasItem, slotNum);
						}
						else {
							Bag_Item_And_Swap_With_Mouse(zone, item, slotNum);
						}
					}
					else if (UI_bagSlots.at(slotNum) != emptyBagSlot) {
						Remove_Item_From_Bag(zone, item, mouseHasItem, slotNum);
					}
				}
			}
		}
	}

	namespace Equipment_UI {
		namespace {
			DataTypes::f2d equipmentOffsetColumn1 = {32.0f, 32.0f};
			DataTypes::f2d equipmentOffsetColumn2 = {452.0f, 32.0f};
			int verticalSpaceBetweenEquipSlots = 10;
			DataTypes::i2d numOfSlots = {2, 5};
			float iEquipmentSlotPixelSize = 81.0f;
			// 452 in the x
			// 10 between in the y
			SDL_FRect screenEquipment = { defaultScreenPosition.x, defaultScreenPosition.y, 490.0f, 1080.0f };
			entt::entity emptyEquipSlot;
			std::map<Item_Type, entt::entity>equippedItems{
				{ Item_Type::helm, emptyEquipSlot },
				{ Item_Type::neck, emptyEquipSlot },
				{ Item_Type::shoulders, emptyEquipSlot },
				{ Item_Type::chest, emptyEquipSlot },
				{ Item_Type::weapon, emptyEquipSlot },
				{ Item_Type::gloves, emptyEquipSlot },
				{ Item_Type::belt, emptyEquipSlot },
				{ Item_Type::legs, emptyEquipSlot },
				{ Item_Type::boots, emptyEquipSlot },
				{ Item_Type::shield, emptyEquipSlot }
			};
			std::map<Item_Type, SDL_FRect>equippedItemsRect{
				{ Item_Type::helm, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize} },
				{ Item_Type::neck, {defaultScreenPosition.x + equipmentOffsetColumn1.x , defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize)+10.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
				{ Item_Type::shoulders, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 2.0f) + 20.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}  },
				{ Item_Type::chest, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 3.0f) + 30.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}  },
				{ Item_Type::weapon, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 4.0f) + 40.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}  },
				{ Item_Type::gloves, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}  },
				{ Item_Type::belt, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize)+10.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}  },
				{ Item_Type::legs, {defaultScreenPosition.x + equipmentOffsetColumn2.x,   defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 2.0f) + 20.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}  },
				{ Item_Type::boots, {defaultScreenPosition.x + equipmentOffsetColumn2.x,  defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 3.0f) + 30.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}  },
				{ Item_Type::shield, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 4.0f) + 40.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}  }
			};
		}

		void Create_Equipment_UI(entt::registry& zone) {
			emptyEquipSlot = Graphics::Create_Icon_Entity(zone, Graphics::emptyBagIcon, Graphics::bagSlotBorder);
			for (auto& itemSlot : equippedItems) {
				itemSlot.second = emptyEquipSlot;
			}
		}

		void Unequip_Item(entt::registry& zone, entt::entity& item, bool& mouseHasItem, const Item_Type& itemType) {
			item = equippedItems[itemType];
			equippedItems[itemType] = emptyEquipSlot;
			mouseHasItem = true;
			zone.emplace<Component::On_Mouse>(item);
			zone.remove<Component::Inventory>(item);
		}
		void Equip_Item(entt::registry& zone, entt::entity& item, bool& mouseHasItem, Item_Type& itemType) {
			equippedItems[itemType] = item;
			mouseHasItem = false;
			zone.remove<Component::On_Mouse>(item);
			zone.emplace<Component::Inventory>(item);
		}
		void Equip_Item_And_Swap_With_Mouse(entt::registry& zone, entt::entity& item, Item_Type& itemType) {
			entt::entity itemInSlot = equippedItems[itemType];
			equippedItems[itemType] = item;
			zone.remove<Component::On_Mouse>(item);
			zone.emplace<Component::Inventory>(item);
			item = itemInSlot;
			zone.emplace<Component::On_Mouse>(item);
			zone.remove<Component::Inventory>(item);
		}

		bool Mouse_Inside_Equipment_Screen(entt::registry& zone, Component::Camera& camera, SDL_FPoint& screenCursor) {
			//SDL_Rect equipmentScreen = Camera_Control::Convert_Rect_To_Scale(zone, screenEquipment, camera);
			return Utilities::bPoint_RectIntersect(screenCursor, screenEquipment);
		}

		bool Interact_With_Equipment(entt::registry& zone, entt::entity& item, SDL_FPoint& mousePoint, bool& mouseHasItem, Component::Camera &camera) {
			/*could be injected instead*/

			if (Mouse_Inside_Equipment_Screen(zone, camera, mousePoint)) {
				//check with item type with a search of where the mouse and and the rects of the equip slots
				if (mouseHasItem) {
					auto& itemType = zone.get<Item_Type>(item);
					/*
					////////////////
					*/
					SDL_FRect slotRect = equippedItemsRect[itemType];
					SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(zone, slotRect, camera);

					if (Mouse::bRect_inside_Cursor(scaledSlot)) {
						if (equippedItems[itemType] == emptyEquipSlot) {
							Equip_Item(zone, item, mouseHasItem, itemType);
							return true;
						}
						else {
							Equip_Item_And_Swap_With_Mouse(zone, item, itemType);
							return true;
						}
					}
				}

				else {
					for (auto& itemSlot : equippedItemsRect) {
						SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(zone, itemSlot.second, camera);
						if (Mouse::bRect_inside_Cursor(scaledSlot)) {
							if (equippedItems[itemSlot.first] != emptyEquipSlot) {
								Unequip_Item(zone, item, mouseHasItem, itemSlot.first);
								return true;
							}
						}
					}
				}
			}
			return false;
		}

		entt::entity Get_Equip_Slot(entt::registry& zone, Component::Camera& camera) {
			for (auto& item : equippedItemsRect) {
				SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(zone, item.second, camera);
				if (Mouse::bRect_inside_Cursor(scaledSlot)) {
					return equippedItems[item.first];
				}
			}
			return emptyEquipSlot;
		}

		void Render_Equipment_Slot(entt::registry& zone, SDL_Renderer *renderer, Component::Camera &camera) {
			for (auto &slot : equippedItems) {
				auto &icon = zone.get<Component::Icon>(slot.second);
				SDL_FRect slotRect = equippedItemsRect[slot.first];

				SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(zone, slotRect, camera);
				SDL_SetTextureAlphaMod(icon.pTexture, 255);
				SDL_SetTextureAlphaMod(icon.pIconRarityBorder, 255);
				SDL_SetTextureAlphaMod(icon.pIconBorder, 255);
				//SDL_RenderCopy(renderer, icon.pBackground, &icon.clipSprite, &scaledSlot);
				SDL_RenderCopyF(renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
				SDL_RenderCopyF(renderer, icon.pIconRarityBorder, &icon.clipIcon, &scaledSlot);
				SDL_RenderCopyF(renderer, icon.pIconBorder, &icon.clipIcon, &scaledSlot);
			}			
		}
		void Update_Equipment_Position(entt::registry& zone, Component::Camera& camera) {
			SDL_FRect equipment = { defaultScreenPosition.x + 28, defaultScreenPosition.y + 28, 506, 450 };
			screenEquipment = Camera_Control::Convert_FRect_To_Scale(zone, equipment, camera);
		}
	}

	bool Swap_Item_In_Bag_For_Equipped(entt::registry& zone, SDL_FPoint& screenCursor, Component::Camera &camera) {
		int slotNum = Bag_UI::Get_Bag_Slot(zone, screenCursor, camera);
		entt::entity itemInSlot = Bag_UI::UI_bagSlots[slotNum];
		if (itemInSlot != Bag_UI::emptyBagSlot) {
			auto type = zone.get<Item_Type>(itemInSlot);
			entt::entity equippedItem = Equipment_UI::equippedItems[type];
			Equipment_UI::equippedItems[type] = itemInSlot;
			if (equippedItem == Equipment_UI::emptyEquipSlot) {
				Bag_UI::UI_bagSlots[slotNum] = Bag_UI::emptyBagSlot;
			} 
			else {
				Bag_UI::UI_bagSlots[slotNum] = equippedItem;
			}
			return true;
		}
		return false;
	}

	void Drop_Item_If_On_Mouse(entt::registry& zone, Component::Camera &camera, entt::entity& item_ID, bool& isItemCurrentlyHeld) {
        if (isItemCurrentlyHeld == true) {
            auto view = zone.view<Component::Input, Component::Position>();
            for (auto entity: view) {
                auto &entityPosition = view.get<Component::Position>(entity);
                auto &scale = zone.get<Component::Scale>(item_ID).scale;
                auto &itemPosition = zone.get<Component::Position>(item_ID);
                auto& offset = zone.get<Component::Sprite_Offset>(item_ID);
                itemPosition = entityPosition;
                zone.emplace<Ground_Item>(item_ID,
                                          itemPosition.x - (offset.x),
                                          itemPosition.y - (offset.y),
                                          offset.x * 2.0f,
                                          offset.y * 2.0f);
                //adds to rendering with the main animation loop
                zone.emplace<Component::Direction>(item_ID, Component::Direction::W);
                // to remove from rendering on mouse
                zone.remove<Component::On_Mouse>(item_ID);
                //allows insertion into quad Trees
                zone.emplace<Component::Radius>(item_ID, offset.x);
            }
        }
        isItemCurrentlyHeld = false;
	}

	void Pick_Up_Item_To_Mouse(entt::registry& zone, entt::entity& item_ID, bool& isItemCurrentlyHeld) {
		if (isItemCurrentlyHeld == false) {
			//removed pickup box from ground
			zone.remove<Ground_Item>(item_ID);
			//removes for main rendering loop
			zone.remove<Component::Direction>(item_ID);
			//to render on mouse
			zone.emplace<Component::On_Mouse>(item_ID);
			Mouse::mouseItem = item_ID;
			Mouse::itemCurrentlyHeld = true;
			//removes from quad tree
            auto &radius = zone.get<Component::Radius>(item_ID);
            auto &position = zone.get<Component::Position>(item_ID);
            SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(radius.fRadius, position.x, position.y);
            zone.emplace<Component::Remove_From_Object_Tree>(item_ID, rect);
            //prevents auto reinsertion into quad tree
			zone.remove<Component::Radius>(item_ID);
		}
	}

	bool Pick_Up_Item_To_Mouse_Or_Bag(entt::registry& zone, Component::Item_Pickup &itemData, bool& isItemCurrentlyHeld) {
		// check if mouse is inside item box				
		SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(itemData.radius, itemData.x, itemData.y);

		if (bToggleCharacterUI) { //bag is closed
			Pick_Up_Item_To_Mouse(zone, itemData.item_ID, isItemCurrentlyHeld);
			return true;
		}
		else {
			//find the first slot with a default icon
			for (int i = 0; i < Bag_UI::UI_bagSlots.size(); i++) {
				if (Bag_UI::UI_bagSlots[i] == Bag_UI::emptyBagSlot) {
					Bag_UI::UI_bagSlots[i] = itemData.item_ID;
                    zone.emplace<Component::Remove_From_Object_Tree>(itemData.item_ID, rect);
					//removed pickup box from ground
					zone.remove<Ground_Item>(itemData.item_ID);
					//removes for main rendering loop
					zone.remove<Component::Direction>(itemData.item_ID);
					zone.emplace_or_replace<Component::Inventory>(itemData.item_ID);
					//prevents we insertion to quad tree
					zone.remove<Component::Radius>(itemData.item_ID);
					return true;
				}
			}
            //set overburdened message here here
            //cane use a nameplate to write it out above sprites head
            std::cout << "I am overburdened" << std::endl;
            return false;
		}
	}

	void Render_UI(entt::registry& zone, SDL_Renderer* renderer, Component::Camera &camera) {
		if (bToggleCharacterUI) {			
			//render UI
			Character_UI = Camera_Control::Convert_FRect_To_Scale(zone, defaultScreenPosition, camera);
			Graphics::Render_FRect(Graphics::itsmars_Inventory, &charui, &Character_UI);
			//reder equipment slots
			Equipment_UI::Update_Equipment_Position(zone, camera);
			Equipment_UI::Render_Equipment_Slot(zone, renderer, camera);
			//render Items in bag
			Bag_UI::screenBag = Camera_Control::Convert_FRect_To_Scale(zone, Bag_UI::bagRect, camera);
			Bag_UI::Render_Bag_Slot(zone, renderer, camera);
		}
	}

	bool Check_If_Arrived(const float& unitX, const float& unitY, const float& destinationX, const float& destinationY) {
		float accuracy = 5.0f;
		if (unitX + accuracy > destinationX &&
			unitX - accuracy < destinationX &&
			unitY - accuracy < destinationY &&
			unitY + accuracy > destinationY) {
			return true;
		}
		else {
			return false;
		}
	}

	int64_t Player_Move_Poll = 0;

	void Mouse_Move_To_Item() { //calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
		Player_Move_Poll += Timer::timeStep;
		if (Player_Move_Poll >= 200) {
			Player_Move_Poll = 0;
			auto view = World::zone.view<Component::Position, Component::Velocity, Component::Item_Pickup, Component::Action, Component::Moving>();
			for (auto entity : view) {
				const auto& x = view.get<Component::Position>(entity);
				const auto& y = view.get<Component::Position>(entity);
				auto& act = view.get<Component::Action>(entity);
				auto& v = view.get<Component::Velocity>(entity);
				auto& mov = view.get<Component::Item_Pickup>(entity);
				act.state = Component::walk;
				v.magnitude.x = v.speed * (mov.x - x.x);
				v.magnitude.y = v.speed * (mov.y - y.y);

			}
		}
	}

	void Mouse_Move_Arrived_Pickup_Item(entt::registry &zone, bool & isItemCurrentlyHeld) {
		auto view = World::zone.view<Component::Sprite_Sheet_Info, Component::Position, Component::Velocity, Component::Action, Component::Item_Pickup>();
		for (auto entity : view) {
			auto& action = view.get<Component::Action>(entity);
			auto& v = view.get<Component::Velocity>(entity);
			const auto& position = view.get<Component::Position>(entity);
			auto& itemData = view.get<Component::Item_Pickup>(entity);
			auto& sheetData = view.get<Component::Sprite_Sheet_Info>(entity);
			if (Check_If_Arrived(position.x, position.y, itemData.x, itemData.y)) {
				if (action.state == Component::walk) {
					v.magnitude.x = 0.0f;
					v.magnitude.y = 0.0f;
					action.state = Component::idle;
                    sheetData.currentFrame = 0;
                    World::zone.remove<Component::Moving>(entity);
				}


                //pickup Item
                Pick_Up_Item_To_Mouse_Or_Bag(zone, itemData, isItemCurrentlyHeld);
				World::zone.remove<Component::Item_Pickup>(entity);
			}
		}
	}

	void Move_To_Item_Routine(entt::registry& zone, bool isItemCurrentlyHeld) {
		Mouse_Move_To_Item();
		Mouse_Move_Arrived_Pickup_Item(zone, isItemCurrentlyHeld);
	}

	//run at game start to init bag vector
	void Init_UI(entt::registry& zone) {
		Items::Init_Item_Data();
		Bag_UI::Create_Bag_UI(zone);
		Equipment_UI::Create_Equipment_UI(zone);
	}
}