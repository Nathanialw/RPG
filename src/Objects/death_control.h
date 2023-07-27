#pragma once
#include "action_components.h"
#include "components.h"
#include "graphics.h"
#include "rendering_components.h"
#include "create_entities.h"
#include "spritesheet_structs.h"
#include "world.h"
#include "item_components.h"
#include "utilities.h"
#include "game_objects.h"
#include <cstdlib>

namespace Death_Component {
  struct Corpse {

  };
}

namespace Death_Control {

  bool Death_Sequence (Component::Direction &direction, entt::entity entity, Component::Scale &scale, Rendering_Components::Sprite_Sheet_Info &sheetData, Action_Component::Action& action, int &numFrames) {
    if (action.state == Action_Component::dying) {
      if (action.frame < numFrames - 1) {
	action.frame++;
	// and if it is an item
	if (action.frame == numFrames - 1) {
	  if (sheetData.sheetData) {
	    if (World::zone.get<Component::Entity_Type>(entity) == Component::Entity_Type::item) {

	    }
	    else if (World::zone.any_of<Item_Component::Equipment>(entity)) {
	      World::zone.emplace<Component::Drop_Equipment>(entity);
	      World::zone.emplace<Death_Component::Corpse>(entity);
	    }
	    else {
	      Utilities::Log("Death_Sequence() a monster died and it does nothing yet");
	      World::zone.emplace<Death_Component::Corpse>(entity);
	    }
	  }
	}
      }
      else {
	Action_Component::Set_State(action, Action_Component::dead);
      }
      return true;
    }
    else if ( action.state == Action_Component::dead) {
      if (action.frame < numFrames - 1) {
	action.frame++;
      }
      return true;
    }
    return false;
  }

  void isDead(entt::registry& zone) {
    auto view = zone.view<Action_Component::Action, Component::Health, Component::Position, Component::Radius, Rendering_Components::Sprite_Offset, Component::Body, Component::In_Object_Tree, Component::Direction>(entt::exclude<Component::Spell>);
    for (auto entity: view) {
      auto &health = view.get<Component::Health>(entity);
      if (health.currentHealth <= 0) {
	auto &action = view.get<Action_Component::Action>(entity);
	Action_Component::Set_State(action, Action_Component::dying);
	//                view.get<Component::Sprite_Sheet_Info>(entity).finalFrame = Component::normalFrame;
	auto &position = view.get<Component::Position>(entity);
	auto &radius = view.get<Component::Radius>(entity).fRadius;
	auto &offset = view.get<Rendering_Components::Sprite_Offset>(entity);
	auto &body = view.get<Component::Body>(entity).body;
	auto &inTree = view.get<Component::In_Object_Tree>(entity).inTree;
	auto &direction = view.get<Component::Direction>(entity);

	Collision::world->DestroyBody(body);
	World::zone.remove<Component::Body>(entity);
	auto rect = zone.get<Component::Interaction_Rect>(entity).rect;

	zone.emplace<Component::Remove_From_Object_Tree>(entity, rect);
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
	std::vector<std::vector<tmx::Vector2<float>>> pointVecs;
	tmx::Vector2<float> imageOffset = {0.0f, 0.0f};
	Collision::aabb aabb;
	Component::Line_Segment line;

	int poolIndex = Utilities::Get_Random_Number(1, Game_Objects_Lists::bloodPoolVec.size() - 1);
	Create_Entities::PVG_Building(zone, position.x, position.y, position.x, position.y, Game_Objects_Lists::bloodPoolVec[poolIndex], poolIndex, aabb, pointVecs, line, imageOffset);
	int splatterIndex = Utilities::Get_Random_Number(1, Game_Objects_Lists::bloodSplatterVec.size() - 1);
	Create_Entities::PVG_Building(zone, position.x, position.y, position.x, position.y, Game_Objects_Lists::bloodSplatterVec[splatterIndex], splatterIndex, aabb, pointVecs, line, imageOffset);

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

  void Drop_Equipment_On_Death (entt::registry &zone) {
    auto view = zone.view<Item_Component::Equipment, Component::Drop_Equipment, Component::Position, Component::Direction, Rendering_Components::Sprite_Sheet_Info, Rendering_Components::Sprite_Offset>();
    for (auto entity : view) {
      auto &equipment = view.get<Item_Component::Equipment>(entity);
      auto &position = view.get<Component::Position>(entity);
      auto &direction = view.get<Component::Direction>(entity);
      auto &unitOffset = view.get<Rendering_Components::Sprite_Offset>(entity);
      auto &sheetData = view.get<Rendering_Components::Sprite_Sheet_Info>(entity);
      auto &offset = view.get<Rendering_Components::Sprite_Offset>(entity);

      for (auto item : equipment.equippedItems) {
	if (item.second != emptyEquipSlot) {
	  auto &offset = World::zone.get<Rendering_Components::Sprite_Offset>(item.second);

	  auto &scale = World::zone.get<Component::Scale>(item.second);
	  auto &itemPosition = zone.get<Component::Position>(item.second);

	  offset = unitOffset;
	  itemPosition = position;

	  SDL_Rect clipRect = sheetData.sheetData->at(sheetData.sheet_name).frameList[sheetData.frameIndex].clip;
	  SDL_FRect renderRect = Utilities::Scale_Rect(clipRect, scale.scale);

	  World::zone.emplace<Item_On_Corpse>(item.second, entity, item.first);

	  SDL_FRect frec = Utilities::SDL_Rect_To_SDL_FRect(clipRect);
	  SDL_FRect rec = Utilities::Centre_Rect_On_Position(frec, itemPosition.x, itemPosition.y);

	  World::zone.emplace_or_replace<Item_Component::Ground_Item>(item.second, rec);
	  World::zone.emplace<Component::Interaction_Rect>(item.second, rec.x, rec.y, (float)clipRect.w, (float)clipRect.h);

	  World::zone.emplace_or_replace<Item_Component::Update_Ground_Item>(item.second);
	  //needs radius to be able to be picked up
	  World::zone.emplace<Component::Radius>(item.second, offset.x);
	}
      }
      zone.remove<Component::Drop_Equipment>(entity);
    }
  }

  void Remove_Item_From_Corpse (entt::registry &zone) {

    auto view = zone.view<Item_On_Corpse, Item_Component::Item_Pickup>();
    for (auto item : view) {
      auto &corpse = view.get<Item_On_Corpse>(item);

      auto &equipment = zone.get<Rendering_Components::Equipment_Sprites>(corpse.entity);
      equipment.sheet[(int)corpse.itemType].name = "empty";
      //            equipment.sheet[(int)corpse.itemType].FrameIndex = 0;
      equipment.sheet[(int)corpse.itemType].ItemSheetData = NULL;
      zone.remove<Item_On_Corpse>(item);
      zone.remove<Item_Pickup>(item);
    }

    auto view2 = zone.view<Item_Component::Item_Pickup>(entt::exclude<Item_On_Corpse>);
    for (auto item2 : view2) {
      zone.remove<Item_Pickup>(item2);
    }
  }

  void Set_As_Corpse (entt::registry &zone) {
    auto view = zone.view<Component::Position, Action_Component::Action, Death_Component::Corpse, Rendering_Components::Sprite_Offset>();
    for (auto entity : view) {
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

  void Update_Ground_Box (entt::registry &zone) {
    auto view = zone.view<Component::Interaction_Rect, Item_Component::Ground_Item, Item_Component::Update_Ground_Item>();
    for (auto entity : view) {
      auto &interactionRect = view.get<Component::Interaction_Rect>(entity).rect;
      auto &groundBox = view.get<Item_Component::Ground_Item>(entity).box;
      
      groundBox = interactionRect;
      //            zone.remove<Item_Component::Update_Ground_Item>(entity);
    }
  }

  void Set_Dead(entt::registry& zone) {
    auto view = zone.view<Action_Component::Action, Component::Health>();
    for (auto entity: view) {
      auto &health = view.get<Component::Health>(entity);
      auto &action = view.get<Action_Component::Action>(entity);
      if (health.currentHealth <= 0 && action.state == Action_Component::idle) {
	Action_Component::Set_State(action, Action_Component::dying);
      }
    }
  }

    void RemoveTileObjects(entt::registry& zone) {
        auto view = zone.view<Component::Body, Component::In_Object_Tree, Component::Tile_Index, Component::Reset>();
        for (auto entity: view) {
            auto &body = view.get<Component::Body>(entity).body;

            Collision::world->DestroyBody(body);
            World::zone.remove<Component::Body>(entity);
            auto rect = zone.get<Component::Interaction_Rect>(entity).rect;

            zone.emplace<Component::Remove_From_Object_Tree>(entity, rect);
            Utilities::Log("removing tile obejct");
        }
    }
  
  
  void Dead_Entity_Routine (entt::registry &zone) {
    Set_Dead(zone);
    Update_Ground_Box(zone);
    Drop_Equipment_On_Death(zone);
    Remove_Item_From_Corpse(zone);    
    Set_As_Corpse(zone);
    RemoveTileObjects(zone);
  }
}
