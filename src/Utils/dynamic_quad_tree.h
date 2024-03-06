#pragma once


//#include "dynamic_entity_loader.h"

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
          SDL_RenderCopyF(Graphics::renderer, Graphics::itemBorderMagic, NULL, &rect);
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
        if (Utilities::Rect_Intersect(rArea, p.first)) {
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
          else if (Utilities::Rect_Intersect(rArea, m_rChild[i])) {
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
    using QuadTreeContainer = std::list<QuadTreeItem<OBJECT_TYPE>>;// contains the item itelfs as well as the location of where it is stored in the quad tree
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


}// namespace Dynamic_Quad_Tree
