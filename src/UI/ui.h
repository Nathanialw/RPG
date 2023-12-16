#pragma once
#include "camera.h"
#include "components.h"
#include "entt/entt.hpp"
#include "item_components.h"
#include "menu.h"
#include "spellbook.h"
#include "ui_actionbar.h"
#include "world.h"

namespace UI {

  SDL_Rect charui = {0, 0, 554, 1080};
  SDL_FRect defaultScreenPosition = {100.0f, 100.0f, 554.0f, 1080.0f};
  SDL_FRect Character_UI = {};
  bool bToggleCharacterUI = false;

  namespace Bag_UI {
    namespace {
      entt::entity emptyBagSlot;
      f2 bagoffset = {32.0f, 544.0f};
      f2 numOfSlots = {8.0f, 4.0f};
      int iTotalSlots = (int) numOfSlots.x * (int) numOfSlots.y;
      float iBagSlotPixelSize = 64.0f;
      SDL_FRect bagRect = {defaultScreenPosition.x + bagoffset.x, defaultScreenPosition.y + bagoffset.y, numOfSlots.x *iBagSlotPixelSize, numOfSlots.y *iBagSlotPixelSize};
      SDL_FRect screenBag = {};
      std::vector<entt::entity> UI_bagSlots(iTotalSlots);
    }// namespace

    void Place_Item_In_Bag(entt::registry &zone, entt::entity &item, bool &mouseHasItem, int &slotNum) {
      UI_bagSlots.at(slotNum) = item;
      mouseHasItem = false;
      zone.remove<Component::On_Mouse>(item);
      zone.emplace_or_replace<Component::Inventory>(item);
    }

    void Remove_Item_From_Bag(entt::registry &zone, entt::entity &item, bool &mouseHasItem, int &slotNum) {
      item = UI_bagSlots.at(slotNum);
      UI_bagSlots.at(slotNum) = emptyBagSlot;
      zone.emplace<Component::On_Mouse>(item);
      zone.remove<Component::Inventory>(item);
      mouseHasItem = true;
    }

    void Bag_Item_And_Swap_With_Mouse(entt::registry &zone, entt::entity &item, int &slotNum) {
      entt::entity itemInSlot = UI_bagSlots[slotNum];
      UI_bagSlots[slotNum] = item;
      zone.remove<Component::On_Mouse>(item);
      zone.emplace<Component::Inventory>(item);
      item = itemInSlot;
      zone.emplace<Component::On_Mouse>(item);
      zone.remove<Component::Inventory>(item);
    }

    void Create_Bag_UI(entt::registry &zone) {
      emptyBagSlot = Graphics::Create_Icon_Entity(zone, Graphics::emptyBagIcon, Graphics::bagSlotBorder);
      for (int i = 0; i < (iTotalSlots); i++) {
        UI_bagSlots.at(i) = emptyBagSlot;
      }
    }

    void Toggle_Bag() {
      if (!bToggleCharacterUI) {
        bToggleCharacterUI = true;
      } else {
        bToggleCharacterUI = false;
      }
    }

    //check if the Mouse point is in the rect and which one
    int Get_Bag_Slot(entt::registry &zone, SDL_FPoint &screenCursor, Component::Camera &camera) {
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
          SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
          if (Utilities::bPoint_RectIntersect(screenCursor, scaledSlot)) {
            return slot;
          }
          if (j < (numOfSlots.y - 1)) {
            slot++;
          }
        }
        SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
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
    void Render_Bag_Slot(entt::registry &zone, SDL_Renderer *renderer, Component::Camera &camera) {
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
          auto &icon = zone.get<Component::Icon>(UI_bagSlots.at(slot));
          /*could be injected instead if it was a class object method*/
          SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
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
        auto &icon = zone.get<Component::Icon>(UI_bagSlots.at(slot));
        /*could be injected instead if it was a class object method*/
        SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
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

    bool Is_Cursor_Inside_Bag_Area(entt::registry &zone, Component::Camera &camera, SDL_FPoint &screenCursor) {
      return Utilities::bPoint_RectIntersect(screenCursor, screenBag);
    }

    void Interact_With_Bag(entt::registry &zone, entt::entity &item, SDL_FPoint &mousePoint, bool &mouseHasItem, Component::Camera &camera) {
      if (Mouse::bRect_inside_Cursor(Character_UI)) {
        int slotNum = Get_Bag_Slot(zone, mousePoint, camera);

        if (Is_Cursor_Inside_Bag_Area(zone, camera, mousePoint)) {
          if (mouseHasItem) {
            if (UI_bagSlots[slotNum] == emptyBagSlot) {
              Place_Item_In_Bag(zone, item, mouseHasItem, slotNum);
            } else {
              Bag_Item_And_Swap_With_Mouse(zone, item, slotNum);
            }
          } else if (UI_bagSlots.at(slotNum) != emptyBagSlot) {
            Remove_Item_From_Bag(zone, item, mouseHasItem, slotNum);
          }
        }
      }
    }
  }// namespace Bag_UI

  namespace Equipment_UI {
    namespace {
      f2 equipmentOffsetColumn1 = {32.0f, 32.0f};
      f2 equipmentOffsetColumn2 = {452.0f, 32.0f};
      float iEquipmentSlotPixelSize = 81.0f;
      // 452 in the x
      // 10 between in the y
      SDL_FRect screenEquipment = {defaultScreenPosition.x, defaultScreenPosition.y, 490.0f, 1080.0f};

      std::map<Item_Component::Item_Type, SDL_FRect> equippedItemsRect{
          {Item_Component::Item_Type::helm, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::amulet, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize) + 10.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::shoulders, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 2.0f) + 20.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::chest, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 3.0f) + 30.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::mainhand, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 4.0f) + 40.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::gloves, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::belt, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize) + 10.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::legs, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 2.0f) + 20.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::boots, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 3.0f) + 30.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::offhand, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 4.0f) + 40.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::hair, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 4.0f) + 800.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}}};
    }// namespace

    void Create_Equipment_UI(entt::registry &zone) {
      Item_Component::emptyEquipSlot = Graphics::Create_Icon_Entity(zone, Graphics::emptyBagIcon, Graphics::bagSlotBorder);
      auto view = zone.view<Item_Component::Equipment>();
      for (auto player: view) {
        auto &equipment = view.get<Item_Component::Equipment>(player);
        for (auto &itemSlot: equipment.equippedItems) {
          itemSlot.second = Item_Component::emptyEquipSlot;
        }
      }
    }

    void Unequip_Item(entt::registry &zone, entt::entity &item, bool &mouseHasItem, const Item_Component::Item_Type &itemType, entt::entity &player) {
      auto &equipment = zone.get<Item_Component::Equipment>(player);
      item = equipment.equippedItems[itemType];
      zone.get<Rendering_Components::Unit_Frame_Portrait>(player).gear[(int)itemType].texture = NULL;

      equipment.equippedItems[itemType] = Item_Component::emptyEquipSlot;
      mouseHasItem = true;
      zone.emplace<Component::On_Mouse>(item);
      zone.remove<Component::Inventory>(item);
    }

    void Equip_Item(entt::registry &zone, entt::entity &item, bool &mouseHasItem, Item_Component::Item_Type &itemType, entt::entity &player) {
      auto &equipment = zone.get<Item_Component::Equipment>(player);
      zone.get<Rendering_Components::Unit_Frame_Portrait>(player).gear[(int)itemType] = zone.get<Rendering_Components::Portrait>(item);

      equipment.equippedItems[itemType] = item;
      mouseHasItem = false;
      zone.remove<Component::On_Mouse>(item);
      zone.emplace<Component::Inventory>(item);
    }

    void Equip_Item_And_Swap_With_Mouse(entt::registry &zone, entt::entity &item, Item_Component::Item_Type &itemType, entt::entity &player) {
      auto &equipment = zone.get<Item_Component::Equipment>(player);
      zone.get<Rendering_Components::Unit_Frame_Portrait>(player).gear[(int)itemType] = zone.get<Rendering_Components::Portrait>(item);

      entt::entity itemInSlot = equipment.equippedItems[itemType];
      equipment.equippedItems[itemType] = item;
      zone.remove<Component::On_Mouse>(item);
      zone.emplace<Component::Inventory>(item);
      item = itemInSlot;
      zone.emplace<Component::On_Mouse>(item);
      zone.remove<Component::Inventory>(item);
    }

    bool Mouse_Inside_Equipment_Screen(entt::registry &zone, Component::Camera &camera, SDL_FPoint &screenCursor) {
      //SDL_Rect equipmentScreen = Camera_Control::Convert_Rect_To_Scale(zone, screenEquipment, camera);
      return Utilities::bPoint_RectIntersect(screenCursor, screenEquipment);
    }

    bool Interact_With_Equipment(entt::registry &zone, entt::entity &item, SDL_FPoint &mousePoint, bool &mouseHasItem, Component::Camera &camera, entt::entity &player) {
      /*could be injected instead*/

      if (Mouse_Inside_Equipment_Screen(zone, camera, mousePoint)) {
        //check which item type with a search of where the mouse and the rects of the equip slots
        auto &equipment = zone.get<Item_Component::Equipment>(player);
        if (mouseHasItem) {
          auto &itemType = zone.get<Item_Component::Item_Type>(item);

          SDL_FRect slotRect = equippedItemsRect[itemType];
          SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);

          if (Mouse::bRect_inside_Cursor(scaledSlot)) {

            if (equipment.equippedItems[itemType] == Item_Component::emptyEquipSlot) {
              Equip_Item(zone, item, mouseHasItem, itemType, player);
              return true;
            } else {
              Equip_Item_And_Swap_With_Mouse(zone, item, itemType, player);
              return true;
            }
          }
        }

        else {
          for (auto &itemSlot: equippedItemsRect) {
            SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(itemSlot.second, camera);
            if (Mouse::bRect_inside_Cursor(scaledSlot)) {
              if (equipment.equippedItems[itemSlot.first] != Item_Component::emptyEquipSlot) {
                Unequip_Item(zone, item, mouseHasItem, itemSlot.first, player);
                return true;
              }
            }
          }
        }
      }
      return false;
    }

    entt::entity Get_Equip_Slot(entt::registry &zone, Component::Camera &camera) {
      auto view = zone.view<Component::Input, Item_Component::Equipment>();
      for (auto player: view) {
        auto &equipment = view.get<Item_Component::Equipment>(player);
        for (auto &item: equippedItemsRect) {
          SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(item.second, camera);
          if (Mouse::bRect_inside_Cursor(scaledSlot)) {
            return equipment.equippedItems[item.first];
          }
        }
      }
      return Item_Component::emptyEquipSlot;
    }

    void Render_Equipment_Slot(entt::registry &zone, SDL_Renderer *renderer, Component::Camera &camera, entt::entity &player) {
      auto &equipment = zone.get<Item_Component::Equipment>(player);
      for (auto &slot: equipment.equippedItems) {
        auto &icon = zone.get<Component::Icon>(slot.second);
        SDL_FRect slotRect = equippedItemsRect[slot.first];

        SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
        SDL_SetTextureAlphaMod(icon.pTexture, 255);
        SDL_SetTextureAlphaMod(icon.pIconRarityBorder, 255);
        SDL_SetTextureAlphaMod(icon.pIconBorder, 255);
        //SDL_RenderCopy(renderer, icon.pBackground, &icon.clipSprite, &scaledSlot);
        SDL_RenderCopyF(renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
        SDL_RenderCopyF(renderer, icon.pIconRarityBorder, &icon.clipIcon, &scaledSlot);
        SDL_RenderCopyF(renderer, icon.pIconBorder, &icon.clipIcon, &scaledSlot);
      }
    }

    void Update_Equipment_Position(Component::Camera &camera) {
      SDL_FRect equipment = {defaultScreenPosition.x + 28, defaultScreenPosition.y + 28, 506, 450};
      screenEquipment = Camera_Control::Convert_FRect_To_Scale(equipment, camera);
    }
  }// namespace Equipment_UI

  bool Swap_Item_In_Bag_For_Equipped(entt::registry &zone, SDL_FPoint &screenCursor, Component::Camera &camera, entt::entity &player) {
    auto &equipment = zone.get<Item_Component::Equipment>(player);
    Utilities::Log("swap item");
    int slotNum = Bag_UI::Get_Bag_Slot(zone, screenCursor, camera);
    entt::entity itemInSlot = Bag_UI::UI_bagSlots[slotNum];
    if (itemInSlot != Bag_UI::emptyBagSlot) {
      auto type = zone.get<Item_Component::Item_Type>(itemInSlot);
      entt::entity equippedItem = equipment.equippedItems[type];
      equipment.equippedItems[type] = itemInSlot;
      zone.get<Rendering_Components::Unit_Frame_Portrait>(player).gear[(int)type] = zone.get<Rendering_Components::Portrait>(itemInSlot);

      if (equippedItem == Item_Component::emptyEquipSlot) {
        Bag_UI::UI_bagSlots[slotNum] = Bag_UI::emptyBagSlot;
      } else {
        Bag_UI::UI_bagSlots[slotNum] = equippedItem;
      }
      return true;
    }
    return false;
  }

  void Drop_Item_If_On_Mouse(entt::registry &zone, Component::Camera &camera, entt::entity &item_ID, bool &isItemCurrentlyHeld) {
    if (isItemCurrentlyHeld == true) {
      auto view = zone.view<Component::Input, Component::Position>();
      for (auto entity: view) {
        auto &entityPosition = view.get<Component::Position>(entity);

        auto &itemPosition = zone.get<Component::Position>(item_ID);
        itemPosition = entityPosition;

        auto &sheetData = zone.get<Rendering_Components::Sprite_Sheet_Info>(item_ID);
        auto direction = zone.emplace_or_replace<Component::Direction>(item_ID, (Component::Direction)(rand() % 7 + 1));
        auto &clipRect = sheetData.sheetData->at(sheetData.sheet_name).frameList.at(sheetData.sheetData->at(sheetData.sheet_name).actionFrameData.at(Action_Component::dead).startFrame + (int) direction);

        zone.remove<Component::On_Mouse>(item_ID);
        SDL_FRect frec = Utilities::SDL_Rect_To_SDL_FRect(clipRect.clip);
        SDL_FRect rec = Utilities::Centre_Rect_On_Position(frec, itemPosition.x, itemPosition.y);

        World::zone.emplace_or_replace<Item_Component::Ground_Item>(item_ID, rec);
        World::zone.emplace<Component::Interaction_Rect>(item_ID, rec.x, rec.y, (float) clipRect.clip.w, (float) clipRect.clip.h);

        World::zone.emplace_or_replace<Item_Component::Update_Ground_Item>(item_ID);

        World::zone.emplace_or_replace<Component::Radius>(item_ID, 10.0f);
        auto &action = World::zone.get<Action_Component::Action>(item_ID);
        Action_Component::Set_State(action, Action_Component::Action_State::dying);
      }
    }
    isItemCurrentlyHeld = false;
  }


  void Pick_Up_Item_To_Mouse(entt::registry &zone, entt::entity &item_ID, bool &isItemCurrentlyHeld) {
    if (isItemCurrentlyHeld == false) {
      //removed pickup box from ground
      zone.remove<Item_Component::Ground_Item>(item_ID);
      //removes for main rendering loop
      zone.remove<Component::Direction>(item_ID);
      zone.remove<Component::Radius>(item_ID);
      //to render on mouse
      zone.emplace<Component::On_Mouse>(item_ID);
      Mouse::mouseItem = item_ID;
      Mouse::itemCurrentlyHeld = true;
      //removes from quad tree
      auto &interactionRect = zone.get<Component::Interaction_Rect>(item_ID).rect;
      auto &position = zone.get<Component::Position>(item_ID);
      //prevents auto reinsertion into quad tree
      zone.emplace<Component::Remove_From_Object_Tree>(item_ID, interactionRect);
    }
  }

  bool Pick_Up_Item_To_Mouse_Or_Bag(entt::registry &zone, Component::Pickup_Item &itemData, bool &isItemCurrentlyHeld) {
    // check if mouse is inside item box
    SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(itemData.radius, itemData.x, itemData.y);

    if (bToggleCharacterUI) {//bag is closed
      Pick_Up_Item_To_Mouse(zone, itemData.item_ID, isItemCurrentlyHeld);
      zone.emplace<Item_Component::Item_Pickup>(itemData.item_ID);
      return true;
    } else {
      //find the first slot with a default icon
      for (int i = 0; i < Bag_UI::UI_bagSlots.size(); i++) {
        if (Bag_UI::UI_bagSlots[i] == Bag_UI::emptyBagSlot) {
          Bag_UI::UI_bagSlots[i] = itemData.item_ID;
          auto &rect = zone.get<Component::Interaction_Rect>(itemData.item_ID).rect;
          zone.emplace<Item_Component::Item_Pickup>(itemData.item_ID);
          zone.emplace<Component::Remove_From_Object_Tree>(itemData.item_ID, rect);
          //removed pickup box from ground
          zone.remove<Item_Component::Ground_Item>(itemData.item_ID);
          //removes for main rendering loop
          zone.remove<Component::Direction>(itemData.item_ID);
          zone.remove<Component::Radius>(itemData.item_ID);
          zone.emplace_or_replace<Component::Inventory>(itemData.item_ID);
          return true;
        }
      }
      //set overburdened message here here
      //cane use a nameplate to write it out above sprites head
      std::cout << "I am overburdened" << std::endl;
      return false;
    }
  }

  void Render_UI(entt::registry &zone, SDL_Renderer *renderer, Component::Camera &camera) {
    auto view = zone.view<Component::Input>();
    for (auto player_ID: view) {

      Action_Bar::Render_Action_Bar(zone, camera);

      if (bToggleCharacterUI) {
        //render UI
        Character_UI = Camera_Control::Convert_FRect_To_Scale(defaultScreenPosition, camera);
        Graphics::Render_FRect(Graphics::itsmars_Inventory, {255, 255, 255}, &charui, &Character_UI);
        //reder equipment slots
        Equipment_UI::Update_Equipment_Position(camera);
        Equipment_UI::Render_Equipment_Slot(zone, renderer, camera, player_ID);
        //render Items in bag
        Bag_UI::screenBag = Camera_Control::Convert_FRect_To_Scale(Bag_UI::bagRect, camera);
        Bag_UI::Render_Bag_Slot(zone, renderer, camera);
      }
    }
  }

  bool Check_If_Arrived(const float &unitX, const float &unitY, const float &destinationX, const float &destinationY) {
    float accuracy = 5.0f;
    if (unitX + accuracy > destinationX &&
        unitX - accuracy < destinationX &&
        unitY - accuracy < destinationY &&
        unitY + accuracy > destinationY) {
      return true;
    } else {
      return false;
    }
  }

  float Player_Move_Poll = 0;

  void Mouse_Move_To_Item() {//calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
    Player_Move_Poll += Timer::timeStep;
    if (Player_Move_Poll >= 200) {
      Player_Move_Poll = 0;
      auto view = World::zone.view<Component::Position, Component::Velocity, Component::Pickup_Item, Action_Component::Action, Component::Moving>();
      for (auto entity: view) {
        const auto &x = view.get<Component::Position>(entity);
        const auto &y = view.get<Component::Position>(entity);
        auto &action = view.get<Action_Component::Action>(entity);
        auto &v = view.get<Component::Velocity>(entity);
        auto &mov = view.get<Component::Pickup_Item>(entity);
        Action_Component::Set_State(action, Action_Component::walk);
        v.magnitude.x = v.speed * (mov.x - x.x);
        v.magnitude.y = v.speed * (mov.y - y.y);
      }
    }
  }

  void Mouse_Move_Arrived_Pickup_Item(entt::registry &zone, bool &isItemCurrentlyHeld) {
    auto view = World::zone.view<Component::Position, Component::Velocity, Action_Component::Action, Component::Pickup_Item>();
    for (auto entity: view) {
      auto &action = view.get<Action_Component::Action>(entity);
      auto &v = view.get<Component::Velocity>(entity);
      const auto &position = view.get<Component::Position>(entity);
      auto &itemData = view.get<Component::Pickup_Item>(entity);
      if (Check_If_Arrived(position.x, position.y, itemData.x, itemData.y)) {
        if (action.state == Action_Component::Action_State::walk) {
          v.magnitude.x = 0.0f;
          v.magnitude.y = 0.0f;
          Action_Component::Set_State(action, Action_Component::Action_State::idle);
          World::zone.remove<Component::Moving>(entity);
        }

        //pickup Item
        Pick_Up_Item_To_Mouse_Or_Bag(zone, itemData, isItemCurrentlyHeld);
        World::zone.remove<Component::Pickup_Item>(entity);
      }
    }
  }

  void Move_To_Item_Routine(entt::registry &zone, bool isItemCurrentlyHeld) {
    Mouse_Move_To_Item();
    Mouse_Move_Arrived_Pickup_Item(zone, isItemCurrentlyHeld);
  }

  //run at game start to init bag vector
  //	void Init_UI(entt::registry& zone) {
  //		Items::Init_Item_Data();
  //		Bag_UI::Create_Bag_UI(zone);
  //		Equipment_UI::Create_Equipment_UI(zone);

  //	}
}// namespace UI
