#pragma once
#include "action_components.h"
#include "components.h"
#include "entt/entt.hpp"
#include "item_components.h"
#include "quad_tree.h"
#include "rendering_components.h"
#include "ui.h"

namespace Load {

  void Get_Bust_Textures(entt::registry &zone, int &state, entt::entity &item, Item_Component::Item_Type itemType, Rendering_Components::Body_Frame &bodyFrame, Rendering_Components::Unit_Frame_Portrait &unitPortraitFrame) {
    if (item != Item_Component::emptyEquipSlot[state]) {
      unitPortraitFrame.gear[(int) itemType] = zone.get<Rendering_Components::Portrait>(item);
      bodyFrame.gear[(int) itemType] = zone.get<Rendering_Components::Portrait>(item);
    }
  }

  void Create_Item(entt::registry &zone, entt::entity &item, Component::Position &position, const std::string &name, Item_Component::Item_Stats &itemStats) {
    float scale = 0.7f;
    zone.emplace_or_replace<Component::Scale>(item, scale);
    zone.emplace_or_replace<Action_Component::Action>(item, Action_Component::isStatic);
    zone.emplace_or_replace<Component::Entity_Type>(item, Component::Entity_Type::item);
    zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(item, 100.0f, 100.0f);

    zone.emplace_or_replace<Item_Component::Item_Stats>(item) = itemStats;
    zone.emplace_or_replace<Item_Component::Name>(item, name);
    zone.emplace_or_replace<Component::Position>(item, position.x, position.y);
  }

  entt::entity Copy_item(entt::registry &OldZone, entt::registry &newZone, entt::entity oldItem) {
    entt::entity newItem = newZone.create();
    auto &itemName = OldZone.get<Item_Component::Name>(oldItem);
    auto &itemStats = OldZone.get<Item_Component::Item_Stats>(oldItem);
    auto &position = OldZone.get<Component::Position>(oldItem);//

    newZone.emplace_or_replace<Rendering_Components::Portrait>(newItem) = OldZone.get<Rendering_Components::Portrait>(oldItem);
    newZone.emplace_or_replace<Rendering_Components::Body>(newItem) = OldZone.get<Rendering_Components::Body>(oldItem);
    newZone.emplace_or_replace<Item_Component::Item_Type>(newItem) = OldZone.get<Item_Component::Item_Type>(oldItem);
    newZone.emplace_or_replace<Item_Component::Rarity>(newItem) = OldZone.get<Item_Component::Rarity>(oldItem);
    newZone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(newItem) = OldZone.get<Rendering_Components::Sprite_Sheet_Info>(oldItem);
    newZone.emplace_or_replace<Component::Icon>(newItem) = OldZone.get<Component::Icon>(oldItem);
    newZone.emplace_or_replace<Component::Inventory>(newItem);

    if (OldZone.any_of<Item_Component::Weapon_Type>(oldItem)) newZone.emplace_or_replace<Item_Component::Weapon_Type>(newItem);
    //
    Create_Item(newZone, newItem, position, itemName.name, itemStats);
    return newItem;
  }

  void Copy_Equipment(entt::registry &OldZone, int &state, int previousState, entt::registry &newZone, entt::entity oldUnit, entt::entity &newUnit) {
    auto view = OldZone.view<Component::Input, Item_Component::Equipment, Component::Position, Rendering_Components::Unit_Frame_Portrait, Rendering_Components::Body_Frame>();
    for (auto unit: view) {
      //need to copy each item in the equipment
      //then delete each item from the reqistry
      auto &equipment = view.get<Item_Component::Equipment>(unit);
      Item_Component::Emplace_Equipment(newZone, state, newUnit, equipment.type);
      auto &position = view.get<Component::Position>(unit);

      auto &newEquipment = newZone.get<Item_Component::Equipment>(newUnit);
      auto &bodyFrame = newZone.get<Rendering_Components::Body_Frame>(newUnit);
      auto &unitPortraitFrame = newZone.get<Rendering_Components::Unit_Frame_Portrait>(newUnit);
      for (const auto &oldItem: equipment.equippedItems) {
        if (oldItem.second != Item_Component::emptyEquipSlot[previousState] && OldZone.any_of<Item_Component::Name>(oldItem.second)) {
          Utilities::Log(OldZone.get<Item_Component::Name>(oldItem.second).name);
          newEquipment.equippedItems[oldItem.first] = Copy_item(OldZone, newZone, oldItem.second);
          //          lags the game out, not sure why yet
          //          Get_Bust_Textures(newZone, state, newEquipment.equippedItems[oldItem.first], static_cast<Item_Component::Item_Type>(newEquipment.equippedItems[oldItem.first]), bodyFrame, unitPortraitFrame);
          OldZone.destroy(oldItem.second);
        } else {
          Utilities::Log(Item_Component::Get_Item_Type_String(oldItem.first) + " emptyEquipSlot: " + std::to_string((int) oldItem.first));
        }
      }
    }
  }

  void Copy_Inventory(entt::registry &OldZone, entt::registry &newZone, int oldState, int &newState, entt::entity oldEntity, entt::entity &newEntity) {
    Bag_UI::Create_Bag_UI(newZone, newEntity, newState);
    auto &oldBag = OldZone.get<Component::Bag>(oldEntity).bag;
    auto &newBag = newZone.get<Component::Bag>(newEntity).bag;

    for (int i = 0; i < oldBag.size(); ++i) {
      //if it is an item and not an empty slot
      if (oldBag[i] != Bag_UI::emptyBagSlot[oldState]) {
        if (OldZone.any_of<Item_Component::Name>(oldBag[i])) {
          Utilities::Log(OldZone.get<Item_Component::Name>(oldBag[i]).name);
          newBag[i] = Copy_item(OldZone, newZone, oldBag[i]);
          OldZone.destroy(oldBag[i]);
        } else {
          Utilities::Log("item not copied ID: " + std::to_string((int) newBag[i]) + "  default slot is ID: " + std::to_string((int) Bag_UI::emptyBagSlot[oldState]));
        }
      }
    }
  }

  void Copy_Player(entt::registry &newZone, entt::registry &oldZone, int newState, int oldState, entt::entity newEntity) {
    auto view = oldZone.view<Component::Interaction_Rect, Component::Body, Social_Component::Relationships, Social_Component::Race, Item_Component::Equipment, Rendering_Components::Equipment_Sprites, Component::Health, Component::Input>();
    for (auto playerID: view) {
      auto &relationships = view.get<Social_Component::Relationships>(playerID);
      auto &race = view.get<Social_Component::Race>(playerID);
      auto &equipment_components = view.get<Rendering_Components::Equipment_Sprites>(playerID);
      auto &health = view.get<Component::Health>(playerID);
      newZone.emplace_or_replace<Social_Component::Relationships>(newEntity) = relationships;
      newZone.emplace_or_replace<Social_Component::Race>(newEntity) = race;
      newZone.emplace_or_replace<Rendering_Components::Equipment_Sprites>(newEntity) = equipment_components;
      newZone.emplace_or_replace<Component::Health>(newEntity) = health;

      Copy_Equipment(oldZone, newState, oldState, newZone, playerID, newEntity);
      Copy_Inventory(oldZone, newZone, oldState, newState, playerID, newEntity);

      auto &body = view.get<Component::Body>(playerID).body;
      auto world = Collision::collisionList[oldState];
      world->DestroyBody(body);

      auto &rect = view.get<Component::Interaction_Rect>(playerID).rect;
      oldZone.emplace_or_replace<Component::Remove_From_Object_Tree>(playerID, rect);
      Quad_Tree::Remove_Entity_From_Tree(oldZone, playerID, oldState);
      oldZone.destroy(playerID);
    }
  }
}// namespace Load