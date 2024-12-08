#pragma once

#include "components.h"
#include "graphics.h"
#include "mouse_data.h"

namespace Mouse {

	using namespace Mouse_Struct;

	//  struct On_Mouse {
	//    entt::entity item_ID;
	//    Component::Icon_Type type = Component::Icon_Type::none;
	//  };

	namespace {
		float cursorRadius = 5.0f;
		float Mouse_Selection_Box_x;
		float Mouse_Selection_Box_y;
	}// namespace

	bool bRight_Mouse_Pressed = false;
	bool bLeft_Mouse_Pressed = false;
	float iXMouse;
	float iYMouse;
	SDL_FPoint mousePoint;
	SDL_FPoint screenMousePoint;

	float iXWorld_Mouse;
	float iYWorld_Mouse;
	float Mouse_Selection_Box_x_Display;
	float Mouse_Selection_Box_y_Display;

	void Init_mouse(entt::registry &zone) {
		mouseData.cursor_ID = Graphics::Create_Icon_Entity(zone, Graphics::cursor_0, nullptr, Component::Icon_Type::none);
		Mouse::mouseData.mouseItem = Mouse::mouseData.cursor_ID;
	}

	bool Set_Cursor_As_Cursor(entt::registry &zone) {
		if (!mouseData.itemCurrentlyHeld || mouseData.type == Component::Icon_Type::none) {
			Utilities::Log("Set_Cursor_As_Entity() failed, itemCurrentlyHeld: " + std::to_string((int) mouseData.itemCurrentlyHeld) + ", entity type: " + std::to_string((int) mouseData.type));
			return false;
		}
		zone.remove<Component::On_Mouse>(Mouse_Struct::mouseData.mouseItem);
		mouseData.mouseItem = Mouse_Struct::mouseData.cursor_ID;
		mouseData.itemCurrentlyHeld = false;
		mouseData.type = Component::Icon_Type::none;
		mouseData.name = "";
		mouseData.direction = -1;

		mouseData.tree = Spell_Data::SIZE;
		mouseData.index = -1;
		return true;
	}

	bool Set_Cursor_As_Entity(entt::registry &zone, const entt::entity &entity, const std::string &name, const int &direction, const Component::Icon_Type type) {
		if (mouseData.itemCurrentlyHeld || mouseData.type != Component::Icon_Type::none) {
			Utilities::Log("Set_Cursor_As_Entity() failed, itemCurrentlyHeld: " + std::to_string((int) mouseData.itemCurrentlyHeld) + ", entity type: " + std::to_string((int) mouseData.type));
			return false;
		}
		zone.emplace<Component::On_Mouse>(entity);
		mouseData.mouseItem = entity;
		mouseData.itemCurrentlyHeld = true;
		mouseData.type = type;
		mouseData.direction = direction;
		mouseData.name = name;
		return true;
	}

	bool Set_Cursor_As_Entity(entt::registry &zone, Mouse_Struct::Entity_ID_Direction &entityData, const Component::Icon_Type type) {
		if (mouseData.itemCurrentlyHeld || mouseData.type != Component::Icon_Type::none) {
			Utilities::Log("Set_Cursor_As_Entity() failed, itemCurrentlyHeld: " + std::to_string((int) mouseData.itemCurrentlyHeld) + ", entity type: " + std::to_string((int) mouseData.type));
			return false;
		}
		zone.emplace<Component::On_Mouse>(entityData.entity);
		mouseData.mouseItem = entityData.entity;
		mouseData.itemCurrentlyHeld = true;
		mouseData.type = type;
		mouseData.direction = entityData.direction;
		mouseData.name = entityData.name;
		return true;
	}

	bool Set_Cursor_As_Entity(entt::registry &zone, const entt::entity &entity, const Component::Icon_Type type) {
		if (mouseData.itemCurrentlyHeld || mouseData.type != Component::Icon_Type::none) {
			Utilities::Log("Set_Cursor_As_Entity() failed, itemCurrentlyHeld: " + std::to_string((int) mouseData.itemCurrentlyHeld) + ", entity type: " + std::to_string((int) mouseData.type));
			return false;
		}
		zone.emplace<Component::On_Mouse>(entity);
		mouseData.mouseItem = entity;
		mouseData.itemCurrentlyHeld = true;
		mouseData.type = type;
		mouseData.direction = -1;
		mouseData.name = "";
		return true;
	}

	struct isEntity {
		bool exists;
		entt::entity entity_ID;
	};

	isEntity Swap_Entity_On_Cursor_With_Entity(entt::registry &zone, entt::entity &toMouse, const Component::Icon_Type type) {
		if (!mouseData.itemCurrentlyHeld || mouseData.type != type) {
			Utilities::Log("Set_Cursor_As_Entity() failed, itemCurrentlyHeld: " + std::to_string((int) mouseData.itemCurrentlyHeld) + ", mouse type: " + std::to_string((int) mouseData.type) + ", entity type: " + std::to_string((int) type));
			return {false};
		}
		entt::entity ItemInSlot = toMouse;
		entt::entity fromMouse = mouseData.mouseItem;

		toMouse = Mouse_Struct::mouseData.mouseItem;
		Mouse::Set_Cursor_As_Cursor(zone);
		Mouse::Set_Cursor_As_Entity(zone, ItemInSlot, type);
		return {true, fromMouse};
	};

	bool FRect_inside_Cursor(SDL_FRect &rect) {
		SDL_FRect cursor = {0.0f, 0.0f, 0.0f, 0.0f};
		cursor = {iXWorld_Mouse, iYWorld_Mouse, cursorRadius, cursorRadius};
		return Utilities::Rect_Intersect(cursor, rect);
	}

	bool FRect_inside_Screen_Cursor(SDL_FRect &rect) {
		SDL_FRect cursor = {iXMouse, iYMouse, cursorRadius, cursorRadius};
		return Utilities::Rect_Intersect(cursor, rect);
	}

	bool bRect_inside_Cursor(SDL_FRect &rect) {
		SDL_FRect cursor = {iXMouse, iYMouse, cursorRadius, cursorRadius};
		return Utilities::Rect_Intersect(cursor, rect);
	}

	bool bRect_inside_World_Cursor(SDL_FRect &rect) {
		SDL_FRect cursor = {iXWorld_Mouse, iYWorld_Mouse, cursorRadius, cursorRadius};
		return Utilities::Rect_Intersect(cursor, rect);
	}

	bool Point_Inside_Cursor(float targetX, float targetY) {
		if (iXWorld_Mouse + cursorRadius > targetX &&
			iXWorld_Mouse - cursorRadius < targetX &&
			iYWorld_Mouse - cursorRadius < targetY &&
			iYWorld_Mouse + cursorRadius > targetY) {
			return true;
		} else
			return false;
	}

	bool Mouse_Selection_Box(SDL_FRect target) {

		SDL_FRect selectionRect = {};

		//        bottom-right
		if (Mouse::Mouse_Selection_Box_x < Mouse::iXWorld_Mouse && Mouse::Mouse_Selection_Box_y < Mouse::iYWorld_Mouse) {
			selectionRect = {
					float(Mouse::Mouse_Selection_Box_x),
					float(Mouse::Mouse_Selection_Box_y),
					float(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x),
					float(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y)};
		}
			//        top-right
		else if (Mouse::Mouse_Selection_Box_x < Mouse::iXWorld_Mouse && Mouse::Mouse_Selection_Box_y > Mouse::iYWorld_Mouse) {
			selectionRect = {
					float(Mouse::Mouse_Selection_Box_x),
					(float(Mouse::Mouse_Selection_Box_y) - abs(float(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y))),
					abs(float(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x)),
					abs(float(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y))};
		}
			//        top-left
		else if (Mouse::Mouse_Selection_Box_x > Mouse::iXWorld_Mouse && Mouse::Mouse_Selection_Box_y < Mouse::iYWorld_Mouse) {
			selectionRect = {
					float(Mouse::Mouse_Selection_Box_x) - abs(float(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x)),
					float(Mouse::Mouse_Selection_Box_y),
					abs(float(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x)),
					abs(float(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y))};
		}
			//        bottom-left
		else if (Mouse::Mouse_Selection_Box_x > Mouse::iXWorld_Mouse && Mouse::Mouse_Selection_Box_y > Mouse::iYWorld_Mouse) {
			selectionRect = {
					float(Mouse::Mouse_Selection_Box_x) - abs(float(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x)),
					(float(Mouse::Mouse_Selection_Box_y) - abs(float(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y))),
					abs(float(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x)),
					abs(float(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y))};
		}


		if ((selectionRect.y <= target.y + target.h) &&
			(selectionRect.x <= target.x + target.w) &&
			(selectionRect.y + selectionRect.h >= target.y) &&
			(selectionRect.x + selectionRect.w >= target.x)) {
			return true;
		}
		return false;
	};
}// namespace Mouse
