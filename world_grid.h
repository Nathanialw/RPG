#pragma once
#include <SDL2/SDL.h>
#include "vector"
#include "entt/entt.hpp"
#include "components.h"
#include "graphics.h"



namespace World_Grid {



	constexpr size_t MAX_DEPTH = 8;
	//OBJECT_TYPE = entt::entity but can you used for any data type
	template <typename OBJECT_TYPE>

	class StaticQuadTree {
	public:
		StaticQuadTree(const SDL_FRect size = {0.0f, 0.0f, 100.0f, 100.0f }, const size_t nDepth = 0) {
			m_depth = nDepth;
			resize(size);
		};

		void resize(const SDL_FRect& rArea) {
			clear();
			m_rect = rArea;
			DataTypes::f2d vChildSize = {m_rect.w / 2.0f, m_rect.h / 2.0f};

			SDL_FRect first = { m_rect.x, m_rect.y, vChildSize.x, vChildSize.y };
			SDL_FRect first1 = { m_rect.x + vChildSize.x, m_rect.y, vChildSize.x, vChildSize.y };
			SDL_FRect first2 = { m_rect.x, m_rect.y + vChildSize.y, vChildSize.x, vChildSize.y };
			SDL_FRect first3 = { m_rect.x + vChildSize.x, m_rect.y + vChildSize.y, vChildSize.x, vChildSize.y };

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

		void insert(const OBJECT_TYPE& item, SDL_FRect& itemsize) {
			for (int i = 0; i < 4; i++) {
				if (Utilities::bFrect_Contains_Frect(m_rChild[i], itemsize)) {
					// Have we reached the limit?
					if (m_depth + 1 < MAX_DEPTH) {
						//No, so does the child exist?
						if (!m_pChild[i]) {
							//No, so create it
							m_pChild[i] = std::make_shared<StaticQuadTree<OBJECT_TYPE>>(m_rChild[i], m_depth + 1);
						}
						//Yes, so add item to it
						m_pChild[i]->insert(item, itemsize);
						return;
					}
				}
			}
			//It didn't fit, so item must belong to this quad
			m_pItems.push_back({ itemsize, item });
		}

		std::list<OBJECT_TYPE> search(const SDL_FRect& rArea) {
			std::list<OBJECT_TYPE> listItems;
			search(rArea, listItems);
			return listItems;
		}


		void search(SDL_FRect rArea, std::list<OBJECT_TYPE>& listItems) {
			//First, check for the items belonging to this area, add them to the list
			// If there is a overlap
			for (auto& p : m_pItems) {
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


		void items(std::list<OBJECT_TYPE>& listItems) const {
			// no questions asked, jsut return child items
			for (const auto& p : m_pItems) {
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
		std::array<std::shared_ptr<StaticQuadTree<OBJECT_TYPE>>, 4> m_pChild{};

		//Items which belong to this StaticQuadTree
		std::vector<std::pair<SDL_FRect, OBJECT_TYPE>> m_pItems;

	};



	template <typename OBJECT_TYPE>
	class StaticQuadTreeContainer {
		using QuadTreeContainer = std::list<OBJECT_TYPE>;

	protected:
		QuadTreeContainer m_allItems;
		StaticQuadTree<typename QuadTreeContainer::iterator> root;

	public:
		StaticQuadTreeContainer(const SDL_FRect &size = { 0.0f, 0.0f, 100.0f, 100.0f }, const size_t nDepth = 0) : root(size) {

		}

		void resize(SDL_FRect& rArea) {
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

		void insert(const OBJECT_TYPE& item,  SDL_FRect &itemsiza) {
			//item is stored in container
			m_allItems.push_back(item);

			//Pointer/Area of item is stored in quad tree
			root.insert(std::prev(m_allItems.end()), itemsiza);
		}

		//Returns a std::list of pointers to items within the search area
		std::list<typename QuadTreeContainer::iterator> search( SDL_FRect &rArea) {
			std::list<typename QuadTreeContainer::iterator> listItemPointers;
			root.search(rArea, listItemPointers);
			return listItemPointers;
		}

	};




	struct someObjectWithArea {
		entt::entity entity_ID;
		SDL_FRect rect;
	};


	//fArea is the area being searched for objects
	float fArea = 100.0f;
	StaticQuadTreeContainer<someObjectWithArea>treeObjects;

	void Fill_Quad_Tree(entt::registry &zone) {
		treeObjects.clear();
		SDL_FRect area = { 0.0f, 0.0f, fArea, fArea };
		treeObjects.resize(area);

		auto view = zone.view<Component::Position, Component::Radius>();

		for (auto entity : view) {
			auto& position = view.get<Component::Position>(entity);
			auto& radius = view.get<Component::Radius>(entity).fRadius;

			SDL_FRect area{};
			area.x = position.x - radius;
			area.y = position.y - radius;
			area.w = (radius * 2.0f);
			area.h = (radius * 2.0f);

			someObjectWithArea object{};
			object.entity_ID = entity;
			object.rect = area;

			treeObjects.insert(object, area);

		}

	}

	void Draw_Tree_Rects(entt::registry &zone) {
		auto view = zone.view<Component::Camera>();
		for (auto entity : view) {
			auto& camera = view.get<Component::Camera>(entity);

			for (auto& object : treeObjects.search(camera.screen)) {
				SDL_FRect screenRect = object->rect;
				screenRect.x -= camera.screen.x;
				screenRect.y -= camera.screen.y;
				SDL_RenderDrawRectF(Graphics::renderer, &screenRect);
			}
		}
	}




}
