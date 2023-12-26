#pragma once

#include "action_components.h"
#include "blood.h"
#include "components.h"
#include "create_entities.h"
#include "game_objects.h"
#include "graphics.h"
#include "item_components.h"
#include "rendering_components.h"
#include "spritesheet_structs.h"
#include "utilities.h"
#include "world.h"
#include <cstdlib>

namespace Death_Component {
  struct Corpse {
  };
}// namespace Death_Component

namespace Death_Control {

  bool Death_Sequence(entt::registry &zone, entt::entity entity, Rendering_Components::Sprite_Sheet_Info &sheetData, Action_Component::Action &action, int &numFrames) {
    if (action.state == Action_Component::dying) {
      if (action.frame < numFrames - 1) {
        action.frame++;
        // and if it is an item
        if (action.frame == numFrames - 1) {
          if (sheetData.sheetData) {
            if (zone.get<Component::Entity_Type>(entity) == Component::Entity_Type::item) {
              Utilities::Log("Death_Control::Death_Sequence() Item lands on the ground");
            } else if (zone.any_of<Item_Component::Equipment>(entity)) {
              zone.emplace_or_replace<Component::Drop_Equipment>(entity);
              zone.emplace_or_replace<Death_Component::Corpse>(entity);
            } else {
              Utilities::Log("Death_Sequence() a monster died and it does nothing yet");
              zone.emplace_or_replace<Death_Component::Corpse>(entity);
            }
          }
        }
      } else {
        Action_Component::Set_State(action, Action_Component::dead);
      }
      return true;
    } else if (action.state == Action_Component::dead) {
      if (action.frame < numFrames - 1) {
        action.frame++;
      }
      return true;
    }
    return false;
  }

  void isDead(entt::registry &zone, int &state) {
    auto view = zone.view<Action_Component::Action, Component::Health, Component::Position, Component::Body, Component::Soldier, Component::Renderable>(entt::exclude<Component::Spell>);
    for (auto entity: view) {
      auto &health = view.get<Component::Health>(entity);
      if (health.currentHealth <= 0) {
        health.currentHealth = 0;
        Utilities::Log("dead entity");
        auto &action = view.get<Action_Component::Action>(entity);
        Action_Component::Set_State(action, Action_Component::dying);
        auto &position = view.get<Component::Position>(entity);
        auto &body = view.get<Component::Body>(entity).body;

        auto world = Collision::collisionList[state];
        world->DestroyBody(body);
        zone.remove<Component::Body>(entity);
        auto rect = zone.get<Component::Interaction_Rect>(entity).rect;

        zone.emplace_or_replace<Component::Remove_From_Object_Tree>(entity, rect);
        zone.get<Component::Alive>(entity).bIsAlive = false;
        zone.remove<Component::Commandable>(entity);
        zone.remove<Component::Selected>(entity);
        zone.remove<Component::Moving>(entity);
        zone.remove<Component::Mouse_Move>(entity);
        zone.remove<Component::Velocity>(entity);
        zone.remove<Component::Spellbook>(entity);
        zone.remove<Component::Mass>(entity);
        zone.remove<Component::Sight_Range>(entity);
        zone.remove<Component::Radius>(entity);
        zone.remove<Collision_Component::Dynamic_Collider>(entity);

        // spawn blood
        Blood::Pool(zone, state, position);

        if (zone.any_of<Component::Assigned_To_Formation>(entity)) {
          auto &soldier = zone.get<Component::Assigned_To_Formation>(entity);
          auto &soldier_list = zone.get<Test::Soldiers_Assigned_List>(soldier.iUnit_Assigned_To);
          soldier_list.unitData[soldier.iIndex].bAlive = false;
          zone.remove<Component::Assigned_To_Formation>(entity);
        }
      }
    }
  }

  struct Item_On_Corpse {
    entt::entity entity;
    Item_Component::Item_Type itemType;
  };

  void Drop_Equipment_On_Death(entt::registry &zone, int &state) {
    auto view = zone.view<Item_Component::Equipment, Component::Drop_Equipment, Component::Position, Component::Direction, Rendering_Components::Sprite_Sheet_Info, Rendering_Components::Sprite_Offset>();
    for (auto entity: view) {
      auto &equipment = view.get<Item_Component::Equipment>(entity);
      auto &position = view.get<Component::Position>(entity);
      auto &direction = view.get<Component::Direction>(entity);
      auto &unitOffset = view.get<Rendering_Components::Sprite_Offset>(entity);
      auto &sheetData = view.get<Rendering_Components::Sprite_Sheet_Info>(entity);
      auto &offset = view.get<Rendering_Components::Sprite_Offset>(entity);

      for (auto item: equipment.equippedItems) {
        if (item.second != emptyEquipSlot[state]) {
          auto &offset = zone.get<Rendering_Components::Sprite_Offset>(item.second);

          auto &scale = zone.get<Component::Scale>(item.second);
          auto &itemPosition = zone.get<Component::Position>(item.second);

          offset = unitOffset;
          itemPosition = position;

          SDL_Rect clipRect = sheetData.sheetData->at(sheetData.sheet_name).frameList[sheetData.frameIndex].clip;
          SDL_FRect renderRect = Utilities::Scale_Rect(clipRect, scale.scale);

          zone.emplace_or_replace<Item_On_Corpse>(item.second, entity, item.first);

          SDL_FRect frec = Utilities::SDL_Rect_To_SDL_FRect(clipRect);
          SDL_FRect rec = Utilities::Centre_Rect_On_Position(frec, itemPosition.x, itemPosition.y);

          zone.emplace_or_replace<Item_Component::Ground_Item>(item.second, rec);
          zone.emplace_or_replace<Component::Interaction_Rect>(item.second, rec.x, rec.y, (float) clipRect.w, (float) clipRect.h);

          zone.emplace_or_replace<Item_Component::Update_Ground_Item>(item.second);
          //needs radius to be able to be picked up
          zone.emplace_or_replace<Component::Radius>(item.second, offset.x);
        }
      }
      zone.remove<Component::Drop_Equipment>(entity);
    }
  }

  void Remove_Item_From_Corpse(entt::registry &zone) {

    auto view = zone.view<Item_On_Corpse, Item_Component::Item_Pickup>();
    for (auto item: view) {
      auto &corpse = view.get<Item_On_Corpse>(item);

      auto &equipment = zone.get<Rendering_Components::Equipment_Sprites>(corpse.entity);
      equipment.sheet[(int) corpse.itemType].name = "empty";
      //            equipment.sheet[(int)corpse.itemType].FrameIndex = 0;
      equipment.sheet[(int) corpse.itemType].ItemSheetData = NULL;
      zone.remove<Item_On_Corpse>(item);
      zone.remove<Item_Pickup>(item);
    }

    auto view2 = zone.view<Item_Component::Item_Pickup>(entt::exclude<Item_On_Corpse>);
    for (auto item2: view2) {
      zone.remove<Item_Pickup>(item2);
    }
  }

  void Set_As_Corpse(entt::registry &zone) {
    auto view = zone.view<Component::Position, Action_Component::Action, Death_Component::Corpse, Rendering_Components::Sprite_Offset>();
    for (auto entity: view) {
      auto &position = view.get<Component::Position>(entity);
      auto &offset = view.get<Rendering_Components::Sprite_Offset>(entity);
      auto &action = view.get<Action_Component::Action>(entity);
      Action_Component::Set_State(action, Action_Component::dead);

      //position as background texture
      position.x -= offset.x;
      position.y -= offset.y;
      offset.x = 0.0f;
      offset.y = 0.0f;

      zone.remove<Death_Component::Corpse>(entity);
    }
  }

  void Update_Ground_Box(entt::registry &zone) {
    auto view = zone.view<Component::Interaction_Rect, Item_Component::Ground_Item, Item_Component::Update_Ground_Item>();
    for (auto entity: view) {
      auto &interactionRect = view.get<Component::Interaction_Rect>(entity).rect;
      auto &groundBox = view.get<Item_Component::Ground_Item>(entity).box;

      groundBox = interactionRect;
      //            zone.remove<Item_Component::Update_Ground_Item>(entity);
    }
  }

  void Set_Dead(entt::registry &zone) {
    auto view = zone.view<Action_Component::Action, Component::Health, Component::Soldier, Component::Renderable>();
    for (auto entity: view) {
      auto &health = view.get<Component::Health>(entity);
      auto &action = view.get<Action_Component::Action>(entity);
      if (health.currentHealth <= 0 && action.state == Action_Component::idle) {
        Action_Component::Set_State(action, Action_Component::dying);
      }
    }
  }

  void Dead_Entity_Routine(entt::registry &zone, int &state) {
    Set_Dead(zone);
    Update_Ground_Box(zone);
    Drop_Equipment_On_Death(zone, state);
    Remove_Item_From_Corpse(zone);
    Set_As_Corpse(zone);
  }
}// namespace Death_Control