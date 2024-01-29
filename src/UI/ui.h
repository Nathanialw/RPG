#pragma once
#include "camera.h"
#include "components.h"
#include "entt/entt.hpp"
#include "item_components.h"
#include "mouse_data.h"
#include "world.h"

namespace UI {

  SDL_Rect charui = {0, 0, 554, 1080};
  SDL_FRect defaultScreenPosition = {100.0f, 100.0f, 554.0f, 1080.0f};
  SDL_FRect Character_UI = {};
  bool bToggleCharacterUI = false;

  //  struct Bag {
  //    std::vector<std::vector<entt::entity>> UI_bagSlots(int);
  //  };

  namespace Bag_UI {
    namespace {
      std::vector<entt::entity> emptyBagSlot(World::numZones);
      f2 bagoffset = {32.0f, 544.0f};
      f2 numOfSlots = {8.0f, 4.0f};
      int iTotalSlots = (int) numOfSlots.x * (int) numOfSlots.y;
      float iBagSlotPixelSize = 64.0f;
      SDL_FRect bagRect = {defaultScreenPosition.x + bagoffset.x, defaultScreenPosition.y + bagoffset.y, numOfSlots.x *iBagSlotPixelSize, numOfSlots.y *iBagSlotPixelSize};
      SDL_FRect screenBag = {};
    }// namespace

    void Place_Item_In_Bag(entt::registry &zone, entt::entity &entity, int &state, entt::entity &mouseItem, bool &mouseHasItem, int &slotNum) {
      if (Mouse_Struct::mouseData.type == Component::Icon_Type::item) {
        auto &bag = zone.get<Component::Bag>(entity).bag;
        if (zone.get<Component::On_Mouse>(mouseItem).type == Component::Icon_Type::item) {
          bag.at(slotNum) = mouseItem;
          mouseHasItem = false;
          zone.remove<Component::On_Mouse>(mouseItem);
          zone.emplace_or_replace<Component::Inventory>(mouseItem);
          mouseItem = Mouse::cursor_ID;
          Mouse_Struct::mouseData.type = Component::Icon_Type::none;
          Mouse_Struct::mouseData.mouseItem = Mouse_Struct::mouseData.cursor_ID;
        }
      }
    }

    void Remove_Item_From_Bag(entt::registry &zone, entt::entity &entity, int &state, entt::entity &item, bool &mouseHasItem, int &slotNum) {
      if (Mouse_Struct::mouseData.type == Component::Icon_Type::none) {
        auto &bag = zone.get<Component::Bag>(entity).bag;

        item = bag.at(slotNum);
        bag.at(slotNum) = emptyBagSlot[state];
        auto &mouseItem = zone.emplace_or_replace<Component::On_Mouse>(item);
        mouseItem.type = Component::Icon_Type::item;
        zone.remove<Component::Inventory>(item);
        mouseHasItem = true;
        Mouse_Struct::mouseData.type = Component::Icon_Type::item;
        Mouse_Struct::mouseData.mouseItem = Mouse_Struct::mouseData.cursor_ID;
      }
    }

    void Bag_Item_And_Swap_With_Mouse(entt::registry &zone, int &state, entt::entity &mouseItem_ID, int &slotNum) {
      if (Mouse_Struct::mouseData.type == Component::Icon_Type::item) {
        auto view = zone.view<Component::Bag, Component::Camera>();
        for (auto entity: view) {
          auto &bag = view.get<Component::Bag>(entity).bag;
          if (zone.get<Component::On_Mouse>(mouseItem_ID).type == Component::Icon_Type::item) {
            entt::entity itemInSlot = bag[slotNum];
            bag[slotNum] = mouseItem_ID;
            zone.remove<Component::On_Mouse>(mouseItem_ID);
            zone.emplace_or_replace<Component::Inventory>(mouseItem_ID);
            mouseItem_ID = itemInSlot;
            auto &mouseItem = zone.emplace_or_replace<Component::On_Mouse>(mouseItem_ID);
            mouseItem.type = Component::Icon_Type::item;
            Mouse_Struct::mouseData.mouseItem = mouseItem_ID;
            zone.remove<Component::Inventory>(mouseItem_ID);
          }
        }
      }
    }

    void Create_Bag_UI(entt::registry &zone, entt::entity &entity, int &state) {
      auto &bag = zone.emplace_or_replace<Component::Bag>(entity).bag;
      for (int i = 0; i < (iTotalSlots); i++) {
        bag.at(i) = emptyBagSlot[state];
      }
    }

    void Close_Bag() {
      if (bToggleCharacterUI) {
        bToggleCharacterUI = false;
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
    void Render_Bag_Slot(entt::registry &zone, entt::entity &entity, int &state, SDL_Renderer *renderer, Component::Camera &camera) {
      auto &bag = zone.get<Component::Bag>(entity).bag;

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
          auto &icon = zone.get<Component::Icon>(bag.at(slot));
          /*could be injected instead if it was a class object method*/
          SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
          SDL_SetTextureAlphaMod(icon.pBackground, 255);
          SDL_SetTextureAlphaMod(icon.pTexture, 255);
          SDL_SetTextureAlphaMod(icon.pIconRarityBorder, 255);
          SDL_SetTextureAlphaMod(icon.pIconBorder, 255);
          SDL_RenderCopyF(renderer, icon.pBackground, &icon.clipIcon, &scaledSlot);
          SDL_RenderCopyF(renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
          SDL_RenderCopyF(renderer, icon.pIconRarityBorder, &icon.clipIcon, &scaledSlot);
          if (j < (numOfSlots.y - 1)) {
            slot++;
          }
        }
        auto &icon = zone.get<Component::Icon>(bag.at(slot));
        /*could be injected instead if it was a class object method*/
        SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
        SDL_SetTextureAlphaMod(icon.pBackground, 255);
        SDL_SetTextureAlphaMod(icon.pTexture, 255);
        SDL_SetTextureAlphaMod(icon.pIconRarityBorder, 255);
        SDL_SetTextureAlphaMod(icon.pIconBorder, 255);
        SDL_RenderCopyF(renderer, icon.pBackground, &icon.clipIcon, &scaledSlot);
        SDL_RenderCopyF(renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
        SDL_RenderCopyF(renderer, icon.pIconRarityBorder, &icon.clipIcon, &scaledSlot);
        if (i < (numOfSlots.x - 1)) {
          slot++;
        }
      }
    }

    bool Is_Cursor_Inside_Bag_Area(entt::registry &zone, Component::Camera &camera, SDL_FPoint &screenCursor) {
      return Utilities::bPoint_RectIntersect(screenCursor, screenBag);
    }

    void Interact_With_Bag(entt::registry &zone, entt::entity &entity, int &state, Component::Camera &camera) {
      auto &bag = zone.get<Component::Bag>(entity).bag;

      if (Mouse::bRect_inside_Cursor(Character_UI)) {
        int slotNum = Get_Bag_Slot(zone, Mouse::screenMousePoint, camera);

        if (Is_Cursor_Inside_Bag_Area(zone, camera, Mouse::screenMousePoint)) {
          if (Mouse::itemCurrentlyHeld) {
            if (bag[slotNum] == emptyBagSlot[state]) {
              Place_Item_In_Bag(zone, entity, state, Mouse::mouseItem, Mouse::itemCurrentlyHeld, slotNum);
            } else {
              Bag_Item_And_Swap_With_Mouse(zone, state, Mouse::mouseItem, slotNum);
            }
          } else if (bag.at(slotNum) != emptyBagSlot[state]) {
            Remove_Item_From_Bag(zone, entity, state, Mouse::mouseItem, Mouse::itemCurrentlyHeld, slotNum);
          }
        }
      }
    }
  }// namespace Bag_UI

  namespace Equipment_UI {
    namespace {
      f2 equipmentOffsetColumn1 = {32.0f, 32.0f};
      f2 equipmentOffsetColumn2 = {452.0f, 32.0f};
      //      float iEquipmentSlotPixelSize = 81.0f;
      float iEquipmentSlotPixelSize = 40.5f;
      // 452 in the x
      // 10 between in the y
      SDL_FRect screenEquipment = {defaultScreenPosition.x, defaultScreenPosition.y, 490.0f, 1080.0f};

      std::map<Item_Component::Item_Type, SDL_FRect> equippedItemsRect{
          {Item_Component::Item_Type::helm, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::amulet, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize) + 10.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::shoulders, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 2.0f) + 20.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},

          {Item_Component::Item_Type::size, {defaultScreenPosition.x + equipmentOffsetColumn1.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 2.0f) + 20.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},

          {Item_Component::Item_Type::chest, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 3.0f) + 30.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::mainhand, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 4.0f) + 40.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::quiver, {defaultScreenPosition.x + equipmentOffsetColumn1.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 4.0f) + 40.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::gloves, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::belt, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize) + 10.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::legs, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 2.0f) + 20.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::boots, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 3.0f) + 30.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::offhand, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 4.0f) + 40.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},

          {Item_Component::Item_Type::clothes, {defaultScreenPosition.x + equipmentOffsetColumn1.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn1.y, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::wrist, {defaultScreenPosition.x + equipmentOffsetColumn1.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize) + 10.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::kilt, {defaultScreenPosition.x + equipmentOffsetColumn1.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn1.y + (iEquipmentSlotPixelSize * 3.0f) + 30.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::face, {defaultScreenPosition.x + equipmentOffsetColumn2.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn2.y, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::hood, {defaultScreenPosition.x + equipmentOffsetColumn2.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize) + 10.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::crown, {defaultScreenPosition.x + equipmentOffsetColumn2.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 2.0f) + 20.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::shins, {defaultScreenPosition.x + equipmentOffsetColumn2.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 3.0f) + 30.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::back, {defaultScreenPosition.x + equipmentOffsetColumn2.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 4.0f) + 40.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::ring0, {defaultScreenPosition.x + equipmentOffsetColumn1.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 5.0f) + 50.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::ring1, {defaultScreenPosition.x + equipmentOffsetColumn1.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 5.0f) + 50.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::jewelry0, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 5.0f) + 50.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
          {Item_Component::Item_Type::jewelry1, {defaultScreenPosition.x + equipmentOffsetColumn2.x + iEquipmentSlotPixelSize, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 5.0f) + 50.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}}};
      //            Item_Component::Item_Type::ranged, {defaultScreenPosition.x + equipmentOffsetColumn2.x, defaultScreenPosition.y + equipmentOffsetColumn2.y + (iEquipmentSlotPixelSize * 4.0f) + 800.0f, iEquipmentSlotPixelSize, iEquipmentSlotPixelSize}},
    }// namespace

    void Sheath_Item(entt::registry &zone, int &state, entt::entity &item, bool &mouseHasItem, const Item_Component::Item_Type &itemType, entt::entity &player) {
      //set shield on back
      //change weapong to bow or staff
    }

    void Unequip_Item(entt::registry &zone, int &state, entt::entity &item, bool &mouseHasItem, const Item_Component::Item_Type &itemType, entt::entity &player) {
      if (Mouse_Struct::mouseData.type == Component::Icon_Type::none) {
        auto &equipment = zone.get<Item_Component::Equipment>(player);
        item = equipment.equippedItems[itemType];
        zone.get<Rendering_Components::Unit_Frame_Portrait>(player).gear[(int) itemType].texture = NULL;

        equipment.equippedItems[itemType] = Item_Component::emptyEquipSlot[state];
        mouseHasItem = true;
        auto &mouseItem = zone.emplace_or_replace<Component::On_Mouse>(item);
        mouseItem.type = Component::Icon_Type::item;
        Mouse_Struct::mouseData.type = Component::Icon_Type::item;
        Mouse_Struct::mouseData.mouseItem = item;
        zone.remove<Component::Inventory>(item);
      }
    }

    bool Check_Mouse_Is_item(entt::registry &zone) {
      if (zone.get<Component::On_Mouse>(Mouse::mouseItem).type == Component::Icon_Type::item || zone.get<Component::On_Mouse>(Mouse::mouseItem).type == Component::Icon_Type::none) {
        return true;
      }
      return false;
    }

    void Equip_Item(entt::registry &zone, Item_Component::Item_Type &itemType, entt::entity &player) {
      if (Check_Mouse_Is_item(zone)) {
        if (Mouse_Struct::mouseData.type == Component::Icon_Type::item) {
          auto &equipment = zone.get<Item_Component::Equipment>(player);
          zone.get<Rendering_Components::Unit_Frame_Portrait>(player).gear[(int) itemType] = zone.get<Rendering_Components::Portrait>(Mouse::mouseItem);
          equipment.equippedItems[itemType] = Mouse::mouseItem;
          if (itemType == Item_Component::Item_Type::mainhand) { zone.get<Action_Component::Action>(player).weaponType = zone.get<Item_Component::Weapon_Type>(Mouse::mouseItem); }
          Mouse::mouseItem = Mouse::cursor_ID;
          Mouse::itemCurrentlyHeld = false;
          zone.remove<Component::On_Mouse>(equipment.equippedItems[itemType]);
          zone.emplace_or_replace<Component::Inventory>(equipment.equippedItems[itemType]);
          Mouse_Struct::mouseData.type = Component::Icon_Type::none;
          Mouse_Struct::mouseData.mouseItem = Mouse_Struct::mouseData.cursor_ID;
        }
      }
    }

    void Equip_Item_And_Swap_With_Mouse(entt::registry &zone, Item_Component::Item_Type &itemType, entt::entity &player) {
      if (Check_Mouse_Is_item(zone)) {
        auto &equipment = zone.get<Item_Component::Equipment>(player);
        entt::entity itemInSlot = equipment.equippedItems[itemType];

        equipment.equippedItems[itemType] = Mouse::mouseItem;
        if (itemType == Item_Component::Item_Type::mainhand) { zone.get<Action_Component::Action>(player).weaponType = zone.get<Item_Component::Weapon_Type>(Mouse::mouseItem); }
        zone.get<Rendering_Components::Unit_Frame_Portrait>(player).gear[(int) itemType] = zone.get<Rendering_Components::Portrait>(equipment.equippedItems[itemType]);
        zone.remove<Component::On_Mouse>(equipment.equippedItems[itemType]);
        zone.emplace_or_replace<Component::Inventory>(equipment.equippedItems[itemType]);

        Mouse::mouseItem = itemInSlot;
        zone.remove<Component::Inventory>(Mouse::mouseItem);
        auto &mouseItem = zone.emplace_or_replace<Component::On_Mouse>(Mouse::mouseItem);
        mouseItem.type = Component::Icon_Type::item;
      }
    }

    bool Mouse_Inside_Equipment_Screen(entt::registry &zone, Component::Camera &camera, SDL_FPoint &screenCursor) {
      return Utilities::bPoint_RectIntersect(screenCursor, screenEquipment);
    }

    bool Interact_With_Equipment(entt::registry &zone, int &state, Component::Camera &camera, entt::entity &player) {
      /*could be injected instead*/
      if (Check_Mouse_Is_item(zone)) {
        if (Mouse_Inside_Equipment_Screen(zone, camera, Mouse::screenMousePoint)) {
          //check which item type with a search of where the mouse and the rects of the equip slots
          auto &equipment = zone.get<Item_Component::Equipment>(player);
          if (Mouse::itemCurrentlyHeld) {
            auto itemType = zone.get<Item_Component::Item_Type>(Mouse::mouseItem);

            SDL_FRect slotRect = equippedItemsRect[itemType];
            SDL_FRect scaledSlot;
            bool inside;
            if (itemType == Item_Component::Item_Type::ring) {
              slotRect = equippedItemsRect[Item_Component::Item_Type::ring0];
              scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
              inside = Mouse::bRect_inside_Cursor(scaledSlot);
              itemType = Item_Component::Item_Type::ring0;
              if (!inside) {
                slotRect = equippedItemsRect[Item_Component::Item_Type::ring1];
                scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
                inside = Mouse::bRect_inside_Cursor(scaledSlot);
                itemType = Item_Component::Item_Type::ring1;
              }
            } else if (itemType == Item_Component::Item_Type::jewelry) {
              slotRect = equippedItemsRect[Item_Component::Item_Type::jewelry0];
              scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
              inside = Mouse::bRect_inside_Cursor(scaledSlot);
              itemType = Item_Component::Item_Type::jewelry0;
              if (!inside) {
                slotRect = equippedItemsRect[Item_Component::Item_Type::jewelry1];
                scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
                inside = Mouse::bRect_inside_Cursor(scaledSlot);
                itemType = Item_Component::Item_Type::jewelry1;
              }
            }

            else {
              scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
              inside = Mouse::bRect_inside_Cursor(scaledSlot);
            }

            if (inside) {
              if (equipment.equippedItems[itemType] == Item_Component::emptyEquipSlot[state]) {
                Equip_Item(zone, itemType, player);
                return true;
              } else {
                Equip_Item_And_Swap_With_Mouse(zone, itemType, player);
                return true;
              }
            }
          }

          else {
            for (auto &itemSlot: equippedItemsRect) {
              SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(itemSlot.second, camera);
              if (Mouse::bRect_inside_Cursor(scaledSlot)) {
                if (equipment.equippedItems[itemSlot.first] != Item_Component::emptyEquipSlot[state]) {
                  Unequip_Item(zone, state, Mouse::mouseItem, Mouse::itemCurrentlyHeld, itemSlot.first, player);
                  return true;
                }
              }
            }
          }
        }
      }
      return false;
    }

    entt::entity Get_Equip_Slot(entt::registry &zone, int &state, Component::Camera &camera) {
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
      return Item_Component::emptyEquipSlot[state];
    }

    void Render_Equipment_Slot(entt::registry &zone, int &state, SDL_Renderer *renderer, Component::Camera &camera, entt::entity &player) {
      auto &equipment = zone.get<Item_Component::Equipment>(player);
      for (auto slot: equipment.equippedItems) {
        if (zone.any_of<Component::Icon>(slot.second)) {
          auto &icon = zone.get<Component::Icon>(slot.second);
          SDL_FRect slotRect = equippedItemsRect[slot.first];

          SDL_FRect scaledSlot = Camera_Control::Convert_FRect_To_Scale(slotRect, camera);
          SDL_SetTextureAlphaMod(icon.pBackground, 255);
          SDL_SetTextureAlphaMod(icon.pTexture, 255);
          SDL_SetTextureAlphaMod(icon.pIconRarityBorder, 255);
          SDL_SetTextureAlphaMod(icon.pIconBorder, 255);
          SDL_RenderCopyF(renderer, icon.pBackground, &icon.clipIcon, &scaledSlot);
          SDL_RenderCopyF(renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
          SDL_RenderCopyF(renderer, icon.pIconRarityBorder, &icon.clipIcon, &scaledSlot);
        }
      }
    }

    void Update_Equipment_Position(Component::Camera &camera) {
      SDL_FRect equipment = {defaultScreenPosition.x + 28, defaultScreenPosition.y + 28, 506, 450};
      screenEquipment = Camera_Control::Convert_FRect_To_Scale(equipment, camera);
    }
  }// namespace Equipment_UI

  bool Swap_Weapon(entt::registry &zone, int &state, entt::entity &player, Item_Component::Item_Type &type, Item_Component::Equipment &equipment, entt::entity &itemInSlot, std::array<entt::entity, 32> &bag, int &slotNum) {
    if (type == Item_Component::Item_Type::mainhand) {
      entt::entity equippedItem = equipment.equippedItems[type];
      equipment.equippedItems[type] = itemInSlot;
      zone.get<Action_Component::Action>(player).weaponType = zone.get<Item_Component::Weapon_Type>(itemInSlot);
      zone.get<Rendering_Components::Unit_Frame_Portrait>(player).gear[(int) type] = zone.get<Rendering_Components::Portrait>(itemInSlot);
      if (equippedItem == Item_Component::emptyEquipSlot[state]) {
        bag[slotNum] = Bag_UI::emptyBagSlot[state];
      } else {
        bag[slotNum] = equippedItem;
      }
      return true;
    }
    return false;
  }

  bool Swap_Multislot(int &state, Item_Component::Item_Type &type, Item_Component::Equipment &equipment, entt::entity &itemInSlot, std::array<entt::entity, 32> &bag, int &slotNum) {
    //check in rung slot 1 is open if it is place it in
    if (type == Item_Component::Item_Type::ring) {
      entt::entity equippedItem = equipment.equippedItems[Item_Component::Item_Type::ring0];
      if (equippedItem == Item_Component::emptyEquipSlot[state]) {
        equipment.equippedItems[Item_Component::Item_Type::ring0] = itemInSlot;
        bag[slotNum] = Bag_UI::emptyBagSlot[state];
        return true;
      }
      //check if ring slot 2 is open if it is place it in
      equippedItem = equipment.equippedItems[Item_Component::Item_Type::ring1];
      if (equippedItem == Item_Component::emptyEquipSlot[state]) {
        equipment.equippedItems[Item_Component::Item_Type::ring1] = itemInSlot;
        bag[slotNum] = Bag_UI::emptyBagSlot[state];
        return true;
      }
      //if neither is open swap ring slot 1
      equippedItem = equipment.equippedItems[Item_Component::Item_Type::ring0];
      equipment.equippedItems[Item_Component::Item_Type::ring0] = itemInSlot;
      bag[slotNum] = equippedItem;
      return true;
    }

    if (type == Item_Component::Item_Type::jewelry) {
      entt::entity equippedItem = equipment.equippedItems[Item_Component::Item_Type::jewelry0];
      if (equippedItem == Item_Component::emptyEquipSlot[state]) {
        equipment.equippedItems[Item_Component::Item_Type::jewelry0] = itemInSlot;
        bag[slotNum] = Bag_UI::emptyBagSlot[state];
        return true;
      }
      //check if ring slot 2 is open if it is place it in
      equippedItem = equipment.equippedItems[Item_Component::Item_Type::jewelry1];
      if (equippedItem == Item_Component::emptyEquipSlot[state]) {
        equipment.equippedItems[Item_Component::Item_Type::jewelry1] = itemInSlot;
        bag[slotNum] = Bag_UI::emptyBagSlot[state];
        return true;
      }
      //if neither is open swap ring slot 1
      equippedItem = equipment.equippedItems[Item_Component::Item_Type::jewelry0];
      equipment.equippedItems[Item_Component::Item_Type::jewelry0] = itemInSlot;
      bag[slotNum] = equippedItem;
      return true;
    }
    return false;
  }

  bool Swap_Item_In_Bag_For_Equipped(entt::registry &zone, int &state, SDL_FPoint &screenCursor, Component::Camera &camera, entt::entity &player) {
    auto &bag = zone.get<Component::Bag>(player).bag;
    auto &equipment = zone.get<Item_Component::Equipment>(player);
    int slotNum = Bag_UI::Get_Bag_Slot(zone, screenCursor, camera);
    entt::entity itemInSlot = bag[slotNum];

    if (itemInSlot != Bag_UI::emptyBagSlot[state]) {
      auto type = zone.get<Item_Component::Item_Type>(itemInSlot);
      if (Swap_Multislot(state, type, equipment, itemInSlot, bag, slotNum)) {
        return true;
      } else if (Swap_Weapon(zone, state, player, type, equipment, itemInSlot, bag, slotNum)) {
        return true;
      } else {
        entt::entity equippedItem = equipment.equippedItems[type];
        equipment.equippedItems[type] = itemInSlot;
        zone.get<Rendering_Components::Unit_Frame_Portrait>(player).gear[(int) type] = zone.get<Rendering_Components::Portrait>(itemInSlot);
        if (equippedItem == Item_Component::emptyEquipSlot[state]) {
          bag[slotNum] = Bag_UI::emptyBagSlot[state];
        } else {
          bag[slotNum] = equippedItem;
        }
        return true;
      }
    }
    return false;
  }

  void Drop_Item_If_On_Mouse(entt::registry &zone, Component::Camera &camera, bool &isItemCurrentlyHeld) {
    if (isItemCurrentlyHeld) {
      auto view = zone.view<Component::Input, Component::Position>();
      for (auto entity: view) {
        if (zone.get<Component::On_Mouse>(Mouse::mouseItem).type == Component::Icon_Type::item) {
          if (zone.get<Item_Component::Item_Type>(Mouse::mouseItem) == Item_Component::Item_Type::back) { return; }
          if (zone.get<Item_Component::Item_Type>(Mouse::mouseItem) == Item_Component::Item_Type::quiver) { return; }
          auto &entityPosition = view.get<Component::Position>(entity);
          auto &itemPosition = zone.get<Component::Position>(Mouse::mouseItem);
          itemPosition = entityPosition;
          entt::entity item_ID = Mouse::mouseItem;
          Mouse::mouseItem = Mouse::cursor_ID;

          auto &sheetData = zone.get<Rendering_Components::Sprite_Sheet_Info>(item_ID);
          auto direction = zone.emplace_or_replace<Component::Direction>(item_ID, (Component::Direction)(rand() % 7 + 1));

          //needs to be changed to a sprite of the item laying on the ground
          // does not currently show a ground sprite for an item without a sprite on character
          // items from the back slot do not work
          SDL_Rect clipRect;
          if (sheetData.sheetData) {
            clipRect = sheetData.sheetData->at(sheetData.sheet_name).frameList.at(sheetData.sheetData->at(sheetData.sheet_name).actionFrameData.at(Action_Component::dead).startFrame + (int) direction).clip;
          } else {
            clipRect = {(int) Mouse::iXWorld_Mouse, (int) Mouse::iYWorld_Mouse, 20, 20};
          }

          SDL_FRect frec = Utilities::SDL_Rect_To_SDL_FRect(clipRect);
          zone.remove<Component::On_Mouse>(item_ID);
          SDL_FRect rec = Utilities::Centre_Rect_On_Position(frec, itemPosition.x, itemPosition.y);

          zone.emplace_or_replace<Item_Component::Ground_Item>(item_ID, rec);
          zone.emplace_or_replace<Component::Interaction_Rect>(item_ID, rec.x, rec.y, (float) clipRect.w, (float) clipRect.h);
          zone.emplace_or_replace<Item_Component::Update_Ground_Item>(item_ID);
          zone.emplace_or_replace<Component::Radius>(item_ID, 10.0f);

          //        if (zone.any_of<Component::Remove_From_Object_Tree>(item_ID)) {
          //          Utilities::Log("item still has Remove_From_Object_Tree");
          //          zone.remove<Component::Remove_From_Object_Tree>(item_ID);
          //        }
          //for some reason it does not remove this component in Remove_From_Tree even though it should
          if (zone.any_of<Component::In_Object_Tree>(item_ID)) {
            Utilities::Log("item was still has In_Object_Tree");
            zone.remove<Component::Remove_From_Object_Tree>(item_ID);
            zone.remove<Component::In_Object_Tree>(item_ID);
          }
          auto &action = zone.get<Action_Component::Action>(item_ID);
          Action_Component::Set_State(action, Action_Component::Action_State::dying);
        }
      }
    }
    isItemCurrentlyHeld = false;
    Mouse::mouseItem = Mouse::cursor_ID;
    Mouse_Struct::mouseData.type = Component::Icon_Type::none;
    Mouse_Struct::mouseData.mouseItem = Mouse_Struct::mouseData.cursor_ID;
  }

  void Pick_Up_Item_To_Mouse(entt::registry &zone, entt::entity &item_ID, bool &isItemCurrentlyHeld) {
    if (!isItemCurrentlyHeld) {
      auto &mouseItem = zone.emplace_or_replace<Component::On_Mouse>(item_ID);
      if (Mouse_Struct::mouseData.type == Component::Icon_Type::none) {
        Mouse_Struct::mouseData.type = Component::Icon_Type::item;
        Mouse_Struct::mouseData.mouseItem = item_ID;
        //removed pickup box from ground
        zone.remove<Item_Component::Ground_Item>(item_ID);
        //removes for main rendering loop
        zone.remove<Component::Direction>(item_ID);
        zone.remove<Component::Radius>(item_ID);
        //to render on mouse
        mouseItem.type = Component::Icon_Type::item;
        Mouse::mouseItem = item_ID;
        Mouse::itemCurrentlyHeld = true;
        //removes from quad tree
        if (zone.any_of<Component::Interaction_Rect>(item_ID)) {
          auto &interactionRect = zone.get<Component::Interaction_Rect>(item_ID).rect;
          auto &position = zone.get<Component::Position>(item_ID);
          //prevents auto reinsertion into quad tree
          zone.emplace_or_replace<Component::Remove_From_Object_Tree>(item_ID, interactionRect);
          zone.remove<Component::Interaction_Rect>(item_ID);
        }
        zone.emplace_or_replace<Item_Component::Item_Pickup>(item_ID);
      }
    }
  }

  bool Pick_Up_Item_To_Bag(entt::registry &zone, entt::entity &entity, int &state, Component::Pickup_Item &itemData, bool &isItemCurrentlyHeld) {
    auto &bag = zone.get<Component::Bag>(entity).bag;
    for (auto &slot: bag) {
      if (slot == Bag_UI::emptyBagSlot[state]) {
        slot = itemData.item_ID;
        auto &rect = zone.get<Component::Interaction_Rect>(itemData.item_ID).rect;
        zone.emplace_or_replace<Item_Component::Item_Pickup>(itemData.item_ID);
        zone.emplace_or_replace<Component::Remove_From_Object_Tree>(itemData.item_ID, rect);
        zone.remove<Component::Interaction_Rect>(itemData.item_ID);
        //removed pickup box from ground
        zone.remove<Item_Component::Ground_Item>(itemData.item_ID);
        //removes for main rendering loop
        zone.remove<Component::Direction>(itemData.item_ID);
        zone.remove<Component::Radius>(itemData.item_ID);
        zone.emplace_or_replace<Component::Inventory>(itemData.item_ID);
        return true;
      }
    }
    return false;
  }

  bool Pick_Up_Item_To_Mouse_Or_Bag(entt::registry &zone, entt::entity &entity, int &state, Component::Pickup_Item &itemData, bool &isItemCurrentlyHeld) {
    if (bToggleCharacterUI) {
      Pick_Up_Item_To_Mouse(zone, itemData.item_ID, isItemCurrentlyHeld);
      return true;
    } else {
      if (Pick_Up_Item_To_Bag(zone, entity, state, itemData, isItemCurrentlyHeld)) {
        return true;
      }
    }
    return false;
  }
}// namespace UI
