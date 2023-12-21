#pragma once

#include "components.h"
#include "graphics.h"
#include "ui.h"
#include "base_structs.h"
#include "utilities.h"
#include <SDL2/SDL_rect.h>
#include "mouse_control.h"

using namespace UI;

namespace Equipment {

  namespace {
    DataTypes::i2d slotOffset = {32, 32};
    DataTypes::i2d numOfSlots = {1, 5};

    int iTotalSlots = numOfSlots.x * numOfSlots.y;
    int iBagSlotPixelSize = 80;

    SDL_Rect equipment = {(int) (defaultScreenPosition.x + Bag_UI::bagoffset.x), (int) (defaultScreenPosition.y + Bag_UI::bagoffset.y), numOfSlots.x * iBagSlotPixelSize, numOfSlots.y * iBagSlotPixelSize};
    SDL_Rect screenEquipment = {};

    std::vector<entt::entity> UI_bagSlots(iTotalSlots);
  }

  void Equip_Item(entt::registry &zone, entt::entity &item, bool &mouseHasItem, int &slotNum) {
    if (zone.get<Component::On_Mouse>(item).type == Component::Icon_Type::item) {
      UI_bagSlots.at(slotNum) = item;
      mouseHasItem = false;
      zone.remove<Component::On_Mouse>(item);
    }
  }

  void Unequip_Item(entt::registry &zone, entt::entity &item, bool &mouseHasItem, int &slotNum) {
    item = UI_bagSlots.at(slotNum);
    UI_bagSlots.at(slotNum) = Graphics::defaultIcon;
    auto &mouseItem = zone.emplace<Component::On_Mouse>(item);
    mouseItem.type = Component::Icon_Type::item;
    mouseHasItem = true;
  }

  void Create_Equipment_UI() {
    for (int i = 0; i < (iTotalSlots); i++) {
      UI_bagSlots.at(i) = Graphics::defaultIcon;
    }
  }

  //check if the Mouse point is in the rect and which one
  int Get_Equipment_Slot(SDL_Point &mousePoint, Component::Camera camera) {

    SDL_Rect slotRect = {};
    slotRect.w = iBagSlotPixelSize;
    slotRect.h = iBagSlotPixelSize;
    int i;
    int j;
    int slot = 0;

    for (i = 0; i < numOfSlots.x; i++) {
      slotRect.x = (i * iBagSlotPixelSize) + Bag.x;

      for (j = 0; j < numOfSlots.y; j++) {
        slotRect.y = (j * iBagSlotPixelSize) + Bag.y;

        SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(slotRect, camera);
        SDL_FRect fSlot = Utilities::SDL_Rect_To_SDL_FRect(scaledSlot);
        if (Utilities::bPoint_RectIntersect(Mouse::mousePoint, fSlot)) {
          return slot;
        }
        if (j < (numOfSlots.y - 1)) {
          slot++;
        }
      }
      SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(slotRect, camera);
      SDL_FRect fSlot = Utilities::SDL_Rect_To_SDL_FRect(scaledSlot);
      if (Utilities::bPoint_RectIntersect(Mouse::mousePoint, fSlot)) {
        return slot;
      }
      if (i < (numOfSlots.x - 1)) {
        slot++;
      }
    }
  }

  //check if the Mouse point is in the rect and which one
  void Render_Equipment_Slot(entt::registry &zone, Component::Camera camera) {

    SDL_Rect slotRect = {};
    slotRect.w = iBagSlotPixelSize;
    slotRect.h = iBagSlotPixelSize;
    int i;
    int j;
    int slot = 0;

    for (i = 0; i < numOfSlots.x; i++) {
      slotRect.x = (i * iBagSlotPixelSize) + Bag.x;

      for (j = 0; j < numOfSlots.y; j++) {
        slotRect.y = (j * iBagSlotPixelSize) + Bag.y;

        auto &icon = zone.get<Component::Icon>(UI_bagSlots.at(slot));
        SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(slotRect, camera);
        SDL_RenderCopy(Graphics::renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
        if (j < (numOfSlots.y - 1)) {
          slot++;
        }
      }

      auto icon = zone.get<Component::Icon>(UI_bagSlots.at(slot));
      SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(slotRect, camera);
      SDL_RenderCopy(Graphics::renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
      if (i < (numOfSlots.x - 1)) {
        slot++;
      }
    }
  }

  void Interact_With_Equipment(entt::registry &zone, entt::entity &item, SDL_Point &mousePoint, bool &mouseHasItem, Component::Camera camera) {
    SDL_Point point = Camera_Control::Convert_Point_To_Scale(mousePoint, camera);
    SDL_FPoint screenCursor = Utilities::SDL_FPoint_to_Point(point);
    int slotNum = Bag_UI::Get_Bag_Slot(zone, screenCursor, camera);

    if (Utilities::bPoint_RectIntersect(screenCursor, Bag_UI::screenBag)) {
      if (mouseHasItem) {
        Equipment_UI::Equip_Item(zone, item, mouseHasItem, slotNum);
      } else if (UI_bagSlots.at(slotNum) != Graphics::defaultIcon) {
        Equipment_UI::Unequip_Item(zone, item, mouseHasItem, slotNum);
      }
    }
  }

  void Render_Equipment(entt::registry &zone, Component::Camera &camera) {
    if (bToggleBag) {
      //render Items in bag
      Bag_UI::Render_Bag_Slot(zone, Graphics::renderer, camera);
    }
  }

  //run at game start to init bag vector
  void Init_Equipment() {
    Create_Bag_UI();
  }
}
