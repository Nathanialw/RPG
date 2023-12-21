#pragma once
//#include "components.h"
//#include "dynamic_quad_tree.h"
////#include "graphics.h"
//
//using namespace Dynamic_Quad_Tree;
//
//namespace Quad_Tree_Controller {
//
//
//
//	struct someObjectWithArea {
//		entt::entity entity_ID;
//		SDL_FRect rect;
//	};
//
//
//	//zoneSize is the area being searched for objects
//	SDL_FRect zoneSize = { 0.0f, 0.0f, 10000.0f, 10000.0f };
//	DynamicQuadTreeContainer<someObjectWithArea>treeObjects;
//
//	void Fill_Quad_Tree(entt::registry& zone) {
//
//		treeObjects.resize(zoneSize);
//
//		auto view = zone.view<Component::Position, Component::Radius>();
//
//		for (auto entity : view) {
//			auto& position = view.get<Component::Position>(entity);
//			auto& radius = view.get<Component::Radius>(entity).fRadius;
//
//			someObjectWithArea object{};
//			object.entity_ID = entity;
//			object.rect = Utilities::Get_FRect_From_Point_Radius(radius, position.x, position.y);
//
//			zone.emplace_or_replace<Component::In_Object_Tree>(entity);
//			treeObjects.insert(object, object.rect);
//			
//		}
//
//	}
//
//
//	void Emplace_Objects_In_Quad_Tree(entt::registry& zone) {
//		auto view = zone.view<Component::Position, Component::Radius>(entt::exclude<Component::In_Object_Tree>);
//		for (auto entity : view) {
//			auto& position = view.get<Component::Position>(entity);
//			auto& radius = view.get<Component::Radius>(entity).fRadius;
//
//			someObjectWithArea object{};
//			object.entity_ID = entity;
//			object.rect = Utilities::Get_FRect_From_Point_Radius(radius, position.x, position.y);
//
//			zone.emplace_or_replace<Component::In_Object_Tree>(entity);
//			treeObjects.insert(object, object.rect);
//		}
//	}
//
//	void Update_Quad_Tree(entt::registry& zone) {
//		auto view = zone.view<Component::Position>();
//
//		for (auto object_it = treeObjects.begin(); object_it != treeObjects.end(); ++object_it) {
//			auto& entity = object_it->item;
//
//			//treeObjects.remove(object_it);
//
//			auto& position = view.get<Component::Position>(entity.entity_ID);
//			auto radius = entity.rect.w / 2.0f;
//			entity.rect = Utilities::Get_FRect_From_Point_Radius(radius, position.x, position.y);
//
//			treeObjects.relocate(object_it, entity.rect);
//		}
//	}
//
//	void Draw_Tree_Rects(entt::registry& zone) {
//		auto view = zone.view<Component::Camera>();
//		for (auto entity : view) {
//			auto& camera = view.get<Component::Camera>(entity);
//
//			for (const auto& object : treeObjects.search(camera.screen)) {
//				SDL_FRect screenRect = object->item.rect;
//				screenRect.x -= camera.screen.x;
//				screenRect.y -= camera.screen.y;
//		//		SDL_RenderDrawRectF(Graphics::renderer, &screenRect);
//			}
//		}
//	}
//
//
//	//remove object from quad tree
//
//	//save iterator data to a component
//	//get the iterator from the list assigend to the the object
//	//use that iterator to remove the object from the quad tree/list as well
//}