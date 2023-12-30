#pragma once
#include "Debug/debug_components.h"
#include "collision.h"
#include "dynamic_entity_loader.h"
#include "dynamic_quad_tree.h"
#include "item_components.h"
#include "world.h"

namespace Quad_Tree {

  struct someObjectWithArea {
    entt::entity entity_ID;
    SDL_FRect rect;
  };

  //zoneSize is the area being searched for objects
  //will be attached to the map later
  SDL_FRect zoneSize = {0.0f, 0.0f, 10000.0f, 10000.0f};
  //  DynamicQuadTreeContainer<someObjectWithArea> treeObjects;
  //  std::unordered_map<World::GameState, Dynamic_Quad_Tree::DynamicQuadTreeContainer<someObjectWithArea>> quadTrees;
  std::vector<Dynamic_Quad_Tree::DynamicQuadTreeContainer<someObjectWithArea>> quadTrees(World::numZones);
  float offset = 40.0f;

  void Fill_Quad_Tree(entt::registry &zone, int &state) {
    quadTrees[state].resize(zoneSize);

    auto view = zone.view<Component::Interaction_Rect>();
    for (auto entity: view) {
      auto &interactRect = view.get<Component::Interaction_Rect>(entity);

      someObjectWithArea object{};
      object.entity_ID = entity;
      object.rect = interactRect.rect;

      zone.emplace_or_replace<Component::In_Object_Tree>(entity, true);
      quadTrees[state].insert(object, object.rect);
    }
  }

  float emplaceQuadTree = 0.0f;

  void Emplace_Objects_In_Quad_Tree(entt::registry &zone, int &state) {
    //    emplaceQuadTree += Timer::timeStep;
    //    if (emplaceQuadTree >= 50.0f) {
    //      emplaceQuadTree -= 50.0f;
    auto view = zone.view<Component::Interaction_Rect>(entt::exclude<Component::In_Object_Tree>);
    int i = 0;
    for (auto entity: view) {
      auto &interactRect = view.get<Component::Interaction_Rect>(entity);
      i++;
      someObjectWithArea object{};
      object.entity_ID = entity;
      object.rect = interactRect.rect;

      zone.emplace_or_replace<Component::In_Object_Tree>(entity, true);
      quadTrees[state].insert(object, object.rect);
    }
    //    }
  }

  void Remove_Tile_From_Tree(entt::registry &zone, int &state) {
    auto view = zone.view<Component::In_Object_Tree, Component::Remove_From_Object_Tree, Component::Tile_Index>();
    for (auto &entity: view) {

      if (zone.any_of<Component::Tile_Index>(entity)) {
        int i = view.get<Component::Tile_Index>(entity).i;
        int j = view.get<Component::Tile_Index>(entity).j;
        tilesEntities[0][i][j].created = false;
        //remove from tree
        if (i < 0 || j < 0) {
          Utilities::Log(std::to_string(tilesEntities[0][i][j].objects.size()));
          continue;
        }
        for (auto tileEntity: tilesEntities[0][i][j].objects) {
          if (zone.valid(tileEntity.entity)) {
            if (!zone.any_of<Component::Interaction_Rect>(tileEntity.entity)) {
              Utilities::Log("the entity " + std::to_string((int) tileEntity.entity) + " has no interaction rect");
              if (zone.any_of<Component::Body>(tileEntity.entity)) {
                auto &body = zone.get<Component::Body>(tileEntity.entity).body;
                Collision::collisionList[state]->DestroyBody(body);
                zone.remove<Component::Body>(tileEntity.entity);
                zone.remove<Component::Remove_From_Object_Tree>(entity);
                zone.remove<Component::In_Object_Tree>(entity);
              }
              zone.destroy(tileEntity.entity);
              continue;
            }
          } else {
            //            Utilities::Log("the entity " + std::to_string((int)tileEntity) + " does not exist, i think it's stuck in the tree");
            continue;
          }
          auto &rect = zone.get<Component::Interaction_Rect>(tileEntity.entity).rect;
          for (auto &object: quadTrees[state].search(rect)) {
            if (object->item.entity_ID == tileEntity.entity) {
              quadTrees[state].remove(object);
            }
          }
          //remove from collision
          if (zone.any_of<Component::Body>(tileEntity.entity)) {
            auto &body = zone.get<Component::Body>(tileEntity.entity).body;
            Collision::collisionList[state]->DestroyBody(body);
            zone.remove<Component::Body>(tileEntity.entity);
            zone.remove<Component::Remove_From_Object_Tree>(tileEntity.entity);
            zone.remove<Component::In_Object_Tree>(tileEntity.entity);
            zone.remove<Component::Interaction_Rect>(tileEntity.entity);
            //            Utilities::Log(sizeof(Collision::collisionList[state]));
            //            Utilities::Log(quadTrees[state].size());
          }
          //remove from registry
          zone.destroy(tileEntity.entity);
        }
        zone.remove<Component::Remove_From_Object_Tree>(entity);
        zone.remove<Component::In_Object_Tree>(entity);
      }
    }
    zone.compact<>();
  }

  void Remove_From_Tree(entt::registry &zone, int &state) {
    auto view = zone.view<Component::In_Object_Tree, Component::Remove_From_Object_Tree>();
    for (auto &entity: view) {

      auto &rect = zone.get<Component::Interaction_Rect>(entity).rect;
      for (auto &object: quadTrees[state].search(rect)) {
        if (object->item.entity_ID == entity) {
          quadTrees[state].remove(object);
        }
      }
      //remove from collision
      if (zone.any_of<Component::Body>(entity)) {
        auto &body = zone.get<Component::Body>(entity).body;
        Collision::collisionList[state]->DestroyBody(body);
        zone.remove<Component::Body>(entity);
      }
      zone.destroy(entity);
      //remove from registry
    }
    zone.compact<>();
  }

  void Remove_Entity_From_Tree(entt::registry &zone, entt::entity &entity, int &state) {
    auto &rect = zone.get<Component::Remove_From_Object_Tree>(entity).rect;
    for (auto &object: quadTrees[state].search(rect)) {
      if (object->item.entity_ID == entity) {
        quadTrees[state].remove(object);
        zone.remove<Component::Interaction_Rect>(entity);
        zone.remove<Component::Remove_From_Object_Tree>(entity);
        zone.remove<Component::In_Object_Tree>(entity);
      }
    }
  }

  //  we can iterate through the list and update all entities in the quad tree
  //  		/* does  a quad search for every object to find it's ne space
  //  			seems like the same as just clearing and rebuilding		*/
  //  or
  //
  //  we can iterate through a view and get all entities that have Moving or Collided
  //  and update their positions with a quad search to find them in the tree
  /* only does a quad search for those that moved*/
  float updateQuadTreePosition = 0.0f;

  void Update_Quad_Tree_Positions(entt::registry &zone, int &state) {
    //    updateQuadTreePosition += Timer::timeStep;
    //    if (updateQuadTreePosition >= 50.0f) {
    //      updateQuadTreePosition -= 50.0f;
    auto view = zone.view<Component::Interaction_Rect, Component::In_Object_Tree>();
    //    Utilities::Log(Item_Component::emptyEquipSlot.size());
    for (std::_List_iterator object_it = quadTrees[state].begin(); object_it != quadTrees[state].end(); ++object_it) {
      auto &entity = object_it->item;
      if (!Debug::settings[Debug::Settings::UpdateQuadTreeDebug]) {
        if (zone.any_of<Component::Interaction_Rect>(entity.entity_ID)) {
          auto &interactRect = view.get<Component::Interaction_Rect>(entity.entity_ID);
          entity.rect = interactRect.rect;
          quadTrees[state].relocate(object_it, entity.rect);
        }

        //need to have an actual rect with an offset of the position and a rect the size of the entity
      } else {
        if (zone.any_of<Component::Interaction_Rect>(entity.entity_ID)) {
          auto &interactRect = view.get<Component::Interaction_Rect>(entity.entity_ID);
          //need to have an actual rect with an offset of the position and a rect the size of the entity
          entity.rect = interactRect.rect;
          quadTrees[state].relocate(object_it, entity.rect);
        } else {
          Utilities::Log("entity is in tree but orphaned");
        }
      }
    }
    //    }
  }

  void Draw_Tree_Object_Rects(entt::registry &zone, int &state) {
    if (Debug::settings[Debug::InteractionRects]) {
      auto view = zone.view<Component::Camera>();
      for (auto entity: view) {
        auto &camera = view.get<Component::Camera>(entity);

        int nodes = 0;
        nodes = quadTrees[state].Draw(camera.screen.x, camera.screen.y, nodes);

        for (const auto &object: quadTrees[state].search(camera.screen)) {
          SDL_FRect screenRect = object->item.rect;
          screenRect.x -= camera.screen.x;
          screenRect.y -= camera.screen.y;
          //          SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 255, 255);
          //          SDL_RenderDrawRectF(Graphics::renderer, &screenRect);
          SDL_RenderCopyF(Graphics::renderer, Graphics::itemBorderMagic, NULL, &screenRect);
          //          SDL_FRect interactRect = view2.get<Component::Interaction_Rect>(object->item.entity_ID).rect;
          //          interactRect.x -= camera.screen.x;
          //          interactRect.y -= camera.screen.y;
          //          SDL_SetRenderDrawColor(Graphics::renderer, 255, 255, 0, 255);
          //          SDL_RenderDrawRectF(Graphics::renderer, &interactRect);
        }
        std::cout << "nodes: " << nodes << std::endl;
      }
      //		std::cout << "objects on screen: " << i << std::endl;
    }
  }

  struct Entity_Data {
    bool b = false;
    entt::entity entity_ID;
  };

  Entity_Data Entity_vs_Mouse_Collision(entt::registry &zone, SDL_FRect &entityRect, int &state) {
    for (const auto &object: quadTrees[state].search(entityRect)) {
      //prevents player from returning themselves from the quadtree
      //should probably make the player entity ID a constant saved somewhere, instead of grabbing it from a view every time
      auto view = zone.view<Component::Input>();
      for (auto player: view) {
        if (object->item.entity_ID == player) {
          continue;
        } else {
          return {true, object->item.entity_ID};
        }
      }
    }
    return {false};
  }

  std::vector<entt::entity> Get_Nearby_Entities(entt::registry &zone, SDL_FRect &entityRect, int &state) {
    std::vector<entt::entity> entityData;

    for (const auto &object: quadTrees[state].search(entityRect)) {
      entityData.emplace_back(object->item.entity_ID);
    }
    entityData.shrink_to_fit();
    return entityData;
  }

  Entity_Data Entity_vs_QuadTree_Collision(entt::registry &zone, SDL_FRect &entityRect, int &state) {
    for (const auto &object: quadTrees[state].search(entityRect)) {
      if (Utilities::bFRect_Intersect(entityRect, object->item.rect)) {
        return {true, object->item.entity_ID};
      } else {
        continue;
      }
    }
    return {false};
  }

  void Update_Tree_Routine(entt::registry &zone, int &state) {
    Emplace_Objects_In_Quad_Tree(zone, state);
    Update_Quad_Tree_Positions(zone, state);
    Draw_Tree_Object_Rects(zone, state);
    Remove_Tile_From_Tree(zone, state);
    Remove_From_Tree(zone, state);
    // draw rects
  }
}// namespace Quad_Tree