#pragma once

#include <SDL2/SDL.h>
#include "vector"
#include "entt/entt.hpp"
#include "utilities.h"
#include "graphics.h"
#include "dynamic_entity_loader.h"
#include "debug_components.h"

namespace Dynamic_Quad_Tree {

  constexpr size_t MAX_DEPTH = 10;
  //OBJECT_TYPE = entt::entity but can you used for any data type

  template<typename T>
  struct QuadTreeItemLocation {
    typename std::list<std::pair<SDL_FRect, T>> *container;
    typename std::list<std::pair<SDL_FRect, T>>::iterator iterator;
  };

  template<typename OBJECT_TYPE>
  class DynamicQuadTree {
  public:
    DynamicQuadTree(const SDL_FRect size = {0.0f, 0.0f, 100.0f, 100.0f}, const size_t nDepth = 0) {
      m_depth = nDepth;
      resize(size);
    };

    void resize(const SDL_FRect &rArea) {
      clear();
      m_rect = rArea;
      f2 vChildSize = {m_rect.w / 2.0f, m_rect.h / 2.0f};

      SDL_FRect first = {m_rect.x, m_rect.y, vChildSize.x, vChildSize.y};
      SDL_FRect first1 = {m_rect.x + vChildSize.x, m_rect.y, vChildSize.x, vChildSize.y};
      SDL_FRect first2 = {m_rect.x, m_rect.y + vChildSize.y, vChildSize.x, vChildSize.y};
      SDL_FRect first3 = {m_rect.x + vChildSize.x, m_rect.y + vChildSize.y, vChildSize.x, vChildSize.y};

      m_rChild = {first, first1, first2, first3};
    };

    void clear() {
      m_pItems.clear();
      for (int i = 0; i < 4; i++) {
        if (m_pChild[i]) {
          m_pChild[i]->clear();
        }
        m_pChild[i].reset();
      }
    }

    size_t size() const {
      size_t nCount = m_pItems.size();
      for (int i = 0; i < 4; i++) {
        if (m_pChild[i]) {
          nCount += m_pChild[i]->size();
        }
      }
      return nCount;
    }

    QuadTreeItemLocation<OBJECT_TYPE> insert(const OBJECT_TYPE &item, SDL_FRect &itemsize) {
      for (int i = 0; i < 4; i++) {
        if (Utilities::bFrect_Contains_Frect(m_rChild[i], itemsize)) {
          // Have we reached the limit?
          if (m_depth + 1 < MAX_DEPTH) {
            //No, so does the child exist?
            if (!m_pChild[i]) {
              //No, so create it
              m_pChild[i] = std::make_shared<DynamicQuadTree<OBJECT_TYPE>>(m_rChild[i], m_depth + 1);
            }
            //Yes, so add item to it
            return m_pChild[i]->insert(item, itemsize);
          }
        }
      }
      //It didn't fit, so item must belong to this quad
      m_pItems.push_back({itemsize, item});
      return {&m_pItems, std::prev(m_pItems.end())};
    }

    std::list<OBJECT_TYPE> search(const SDL_FRect &rArea) {
      std::list<OBJECT_TYPE> listItems;
      search(rArea, listItems);
      return listItems;
    }

    int Draw(float &cameraX, float &cameraY, int &nodeCount) {
      for (int i = 0; i < 4; i++) {
        if (m_pChild[i]) {
          SDL_FRect rect = m_rChild[i];
          rect.x -= cameraX;
          rect.y -= cameraY;
          SDL_RenderDrawRectF(Graphics::renderer, &rect);
          nodeCount++;
          m_pChild[i]->Draw(cameraX, cameraY, nodeCount);
        }
      }
      return nodeCount;
    }

    void search(SDL_FRect rArea, std::list<OBJECT_TYPE> &listItems) {
      //First, check for the items belonging to this area, add them to the list
      // If there is a overlap
      for (auto &p: m_pItems) {
        if (Utilities::bFRect_Intersect(rArea, p.first)) {
          listItems.push_back(p.second);
        }
      }

      //Second, recurse through children and see if they can
      // add to the list
      for (int i = 0; i < 4; i++) {
        if (m_pChild[i]) {
          //if child it entirely conatained within area, recursively
          //add all of its children, no need to check bounderies
          if (Utilities::bFrect_Contains_Frect(rArea, m_rChild[i])) {
            m_pChild[i]->items(listItems);
          }
            //if child overlaps with search area then checks need to be made
          else if (Utilities::bFRect_Intersect(rArea, m_rChild[i])) {
            m_pChild[i]->search(rArea, listItems);
          }
        }
      }
    }

    void items(std::list<OBJECT_TYPE> &listItems) const {
      // no questions asked, jsut return child items
      for (const auto &p: m_pItems) {
        listItems.push_back(p.second);
      }
      for (int i = 0; i < 4; i++) {
        if (m_pChild[i]) {
          m_pChild[i]->items(listItems);
        }
      }
    }

    const SDL_FRect area() {
      return m_rect;
    }

  protected:
    size_t m_depth = 0;

    //Area of this StaticQuadTree
    SDL_FRect m_rect;

    //4 child areas of this StaticQuadTree
    std::array<SDL_FRect, 4> m_rChild{};

    //4 potential children of this StaticQuadTree
    std::array<std::shared_ptr<DynamicQuadTree<OBJECT_TYPE>>, 4> m_pChild{};

    //Items which belong to this StaticQuadTree
    std::list<std::pair<SDL_FRect, OBJECT_TYPE>> m_pItems;
  };

  template<typename T>
  struct QuadTreeItem {
    //the item itself
    T item;
    //A "locator" to the container.iterator that points to thisd item's iterator in the top level list
    QuadTreeItemLocation<typename std::list<QuadTreeItem<T>>::iterator> pItem;
  };

  template<typename OBJECT_TYPE>
  class DynamicQuadTreeContainer {
    using QuadTreeContainer = std::list<QuadTreeItem<OBJECT_TYPE>>; // contains the item itelfs as well as the location of where it is stored in the quad tree
  protected:

    QuadTreeContainer m_allItems;
    DynamicQuadTree<typename QuadTreeContainer::iterator> root;

  public:
    DynamicQuadTreeContainer(const SDL_FRect &size = {0.0f, 0.0f, 100.0f, 100.0f}, const size_t nDepth = 0) : root(size) {

    }

    void resize(SDL_FRect &rArea) {
      root.resize(rArea);
    }

    size_t size() const {
      return m_allItems.size();
    }

    bool empty() const {
      return m_allItems.empty();
    }

    void clear() {
      root.clear();
      m_allItems.clear();
    }

    typename QuadTreeContainer::iterator begin() {
      return m_allItems.begin();
    }

    typename QuadTreeContainer::iterator end() {
      return m_allItems.end();
    }

    typename QuadTreeContainer::const_iterator cbegin() {
      return m_allItems.cbegin();
    }

    typename QuadTreeContainer::const_iterator cend() {
      return m_allItems.cend();
    }

    void insert(const OBJECT_TYPE &item, SDL_FRect &itemsiza) {
      QuadTreeItem<OBJECT_TYPE> newItem;
      newItem.item = item;

//      item is stored in container
      m_allItems.push_back(newItem);

      //Pointer/Area of item is stored in quad tree
      m_allItems.back().pItem = root.insert(std::prev(m_allItems.end()), itemsiza);
    }

    // if returns nodeCount + number of nodes in the tree
    int Draw(float &x, float &y, int nodeCount) {
      return root.Draw(x, y, nodeCount);
    }

    //Returns a std::list of pointers to items within the search area
    std::list<typename QuadTreeContainer::iterator> search(SDL_FRect &rArea) {
      std::list<typename QuadTreeContainer::iterator> listItemPointers;
      root.search(rArea, listItemPointers);
      return listItemPointers;
    }

    void remove(typename QuadTreeContainer::iterator &item) {
      //iterator points to a QuadTreeItem
      item->pItem.container->erase(item->pItem.iterator);
      m_allItems.erase(item);
    }

    void relocate(typename QuadTreeContainer::iterator &item, SDL_FRect &itemsize) {
      //iterator points to a QuadTreeItem
      item->pItem.container->erase(item->pItem.iterator);

      //Update the items pointer by reinserting into the quad tree
      item->pItem = root.insert(item, itemsize);
    }
  };

  struct someObjectWithArea {
    entt::entity entity_ID;
    SDL_FRect rect;
  };

  //zoneSize is the area being searched for objects
  //will be attached to the map later
  SDL_FRect zoneSize = {0.0f, 0.0f, 10000.0f, 10000.0f};
  DynamicQuadTreeContainer<someObjectWithArea> treeObjects;
  float offset = 40.0f;

  void Fill_Quad_Tree(entt::registry &zone) {
    treeObjects.resize(zoneSize);

    auto view = zone.view<Component::Interaction_Rect>();
    for (auto entity: view) {
      auto &interactRect = view.get<Component::Interaction_Rect>(entity);

      someObjectWithArea object{};
      object.entity_ID = entity;
      object.rect = interactRect.rect;

      zone.emplace<Component::In_Object_Tree>(entity, true);
      treeObjects.insert(object, object.rect);
    }
  }

  float emplaceQuadTree = 0.0f;

  void Emplace_Objects_In_Quad_Tree(entt::registry &zone) {
    emplaceQuadTree += Timer::timeStep;
    if (emplaceQuadTree >= 50.0f) {
      emplaceQuadTree -= 50.0f;
      auto view = zone.view<Component::Interaction_Rect>(entt::exclude<Component::In_Object_Tree>);
      int i = 0;
      for (auto entity: view) {
        auto &interactRect = view.get<Component::Interaction_Rect>(entity);
        i++;
        someObjectWithArea object{};
        object.entity_ID = entity;
        object.rect = interactRect.rect;

        zone.emplace<Component::In_Object_Tree>(entity, true);
        treeObjects.insert(object, object.rect);
      }
    }
  }

  void Remove_From_Tree(entt::registry &zone) {
    auto view = zone.view<Component::In_Object_Tree, Component::Remove_From_Object_Tree>();
    for (auto &entity: view) {
      auto &rect = view.get<Component::Remove_From_Object_Tree>(entity).rect;
      for (auto &object: treeObjects.search(rect)) {
        if (object->item.entity_ID == entity) {
          treeObjects.remove(object);
        }
      }
      if (zone.any_of<Component::Tile_Index>(entity)) {
        int i = zone.get<Component::Tile_Index>(entity).i;
        int j = zone.get<Component::Tile_Index>(entity).j;
        tilesToRender[i][j].created = false;
        zone.remove<Component::Position>(entity);
        zone.destroy(entity);
      } else {
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

  void Update_Quad_Tree_Positions(entt::registry &zone) {
    updateQuadTreePosition += Timer::timeStep;
    if (updateQuadTreePosition >= 50.0f) {
      updateQuadTreePosition -= 50.0f;
      auto view = zone.view<Component::Interaction_Rect>();
      Debug::treeSize = view.size();

      for (std::_List_iterator object_it = treeObjects.begin(); object_it != treeObjects.end(); ++object_it) {
        auto &entity = object_it->item;
        if (!Debug::settings[Debug::Settings::UpdateQuadTreeDebug]) {
          auto &interactRect = view.get<Component::Interaction_Rect>(entity.entity_ID);

          //need to have an actual rect with an offset of the position and a rect the size of the entity
          entity.rect = interactRect.rect;
          treeObjects.relocate(object_it, entity.rect);
        } else {
          if (zone.any_of<Component::Interaction_Rect>(entity.entity_ID)) {
            auto &interactRect = view.get<Component::Interaction_Rect>(entity.entity_ID);

            //need to have an actual rect with an offset of the position and a rect the size of the entity
            entity.rect = interactRect.rect;

            treeObjects.relocate(object_it, entity.rect);
          } else {
            Utilities::Log("entity is in tree but orphaned");
          }
        }
      }
    }
  }

  void Draw_Tree_Object_Rects(entt::registry &zone) {
    if (Debug::settings[2]) {
      auto view2 = zone.view<Component::Interaction_Rect>();
      auto view = zone.view<Component::Camera>();
      for (auto entity: view) {
        auto &camera = view.get<Component::Camera>(entity);

        int nodes = 0;
        nodes = treeObjects.Draw(camera.screen.x, camera.screen.y, nodes);

        for (const auto &object: treeObjects.search(camera.screen)) {
          SDL_FRect screenRect = object->item.rect;
          screenRect.x -= camera.screen.x;
          screenRect.y -= camera.screen.y;
          SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 255, 255);
          SDL_RenderDrawRectF(Graphics::renderer, &screenRect);
//          SDL_FRect interactRect = view2.get<Component::Interaction_Rect>(object->item.entity_ID).rect;
//          interactRect.x -= camera.screen.x;
//          interactRect.y -= camera.screen.y;
//          SDL_SetRenderDrawColor(Graphics::renderer, 255, 255, 0, 255);
//          SDL_RenderDrawRectF(Graphics::renderer, &interactRect);
        }
        //std::cout << "nodes: " << nodes << std::endl;
      }
      //		std::cout << "objects on screen: " << i << std::endl;
    }
  }

  struct Entity_Data {
    bool b = false;
    entt::entity entity_ID;
  };

  Entity_Data Entity_vs_Mouse_Collision(entt::registry &zone, SDL_FRect &entityRect) {
    for (const auto &object: treeObjects.search(entityRect)) {
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

  std::vector<entt::entity> Get_Nearby_Entities(entt::registry &zone, SDL_FRect &entityRect) {
    std::vector<entt::entity> entityData;

    for (const auto &object: treeObjects.search(entityRect)) {
      entityData.emplace_back(object->item.entity_ID);
    }
    entityData.shrink_to_fit();
    return entityData;
  }

  Entity_Data Entity_vs_QuadTree_Collision(entt::registry &zone, SDL_FRect &entityRect) {
    for (const auto &object: treeObjects.search(entityRect)) {
      if (Utilities::bFRect_Intersect(entityRect, object->item.rect)) {
        return {true, object->item.entity_ID};
      } else {
        continue;
      }
    }
    return {false};
  }

  void Update_Tree_Routine(entt::registry &zone) {
    Emplace_Objects_In_Quad_Tree(zone);
    Update_Quad_Tree_Positions(zone);
    Draw_Tree_Object_Rects(zone);
    Remove_From_Tree(zone);
    // draw rects
  }


}
