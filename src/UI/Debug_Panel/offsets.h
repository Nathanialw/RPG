#pragma once

#include "Collision/collision_load.h"
#include "SDL2/SDL.h"
#include "SQLite_insert.h"
#include "components.h"
#include "mouse_control.h"
#include "string"
#include "ui_elements.h"

namespace Button {

	typedef int (*debugButtons)(Component::Camera &camera, SDL_FRect panel, const std::string &name, float &x, float &y);

	struct {
		bool colliders = false;
		float increment = 1;

		std::string table = "building_exteriors";

		std::string x = "x_offset";
		std::string collider_x = "x_collider_offset";

		std::string y = "y_offset";
		std::string collider_y = "y_collider_offset";
	} offset;


	int Offsets(Component::Camera &camera, SDL_FRect panel, const std::string &name, float &x, float &y) {
		SDL_FRect draw = UI::Update_Scale(camera.scale, panel);
		float a = 15.0f / camera.scale.x;
		float b = 20.0f / camera.scale.x;
		float c = 40.0f / camera.scale.x;
		float d = 45.0f / camera.scale.x;
		float e = 20.0f / camera.scale.x;
		float f = 55.0f / camera.scale.x;
		float g = 60.0f / camera.scale.x;
		float h = 215.0f / camera.scale.x;
		float i = 325.0f / camera.scale.x;
		float j = 110.0f / camera.scale.x;
		SDL_FRect button;


		button = {draw.x + e, draw.y + d, a, b};// left
		if (Mouse::bRect_inside_Cursor(button)) {
			x += offset.increment;
			SQLite_Insert::Update_DB(offset.table, offset.x, name, offset.increment);
			if (offset.colliders) {
				SQLite_Insert::Update_DB(offset.table, offset.collider_x, name, offset.increment);
				Collision::Update_Offset_X(name, offset.increment);
			}
			return 1;
		}
		button = {draw.x + g, draw.y + d, a, b};// right
		if (Mouse::bRect_inside_Cursor(button)) {
			x += -offset.increment;
			SQLite_Insert::Update_DB(offset.table, offset.x, name, -offset.increment);
			if (offset.colliders) {
				SQLite_Insert::Update_DB(offset.table, offset.collider_x, name, -offset.increment);
				Collision::Update_Offset_X(name, -offset.increment);
			}
			return 1;
		}
		button = {draw.x + c, draw.y + c, b, a};//up
		if (Mouse::bRect_inside_Cursor(button)) {
			y += offset.increment;
			SQLite_Insert::Update_DB(offset.table, offset.y, name, offset.increment);
			if (offset.colliders) {
				SQLite_Insert::Update_DB(offset.table, offset.collider_y, name, -offset.increment);
				Collision::Update_Offset_Y(name, offset.increment);
			}
			return 1;
		}
		button = {draw.x + c, draw.y + f, b, a};// down
		if (Mouse::bRect_inside_Cursor(button)) {
			y += -offset.increment;
			SQLite_Insert::Update_DB(offset.table, offset.y, name, -offset.increment);
			if (offset.colliders) {
				SQLite_Insert::Update_DB(offset.table, offset.collider_y, name, offset.increment);
				Collision::Update_Offset_Y(name, -offset.increment);
			}
			return 1;
		}


		button = {draw.x + h, draw.y + d, j, a};//
		if (Mouse::bRect_inside_Cursor(button)) {
			(offset.increment == 1) ? offset.increment = 10 : offset.increment = 1;
			return 1;
		}
		button = {draw.x + i, draw.y + d, j, a};//
		if (Mouse::bRect_inside_Cursor(button)) {
			offset.colliders = !offset.colliders;
			return 1;
		}
		return 0;
	}


}// namespace Button

namespace Label {

	int Offsets(const Component::Camera &camera, SDL_FRect statBox, const FC_Scale &scale, const SDL_Color &color, const std::string &tag) {
		FC_DrawScaleRightColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, tag.c_str());
		statBox.x += 60.0f / camera.scale.y;//45.0f // 325.0f //
		if (Button::offset.increment == 1) {
			FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "1");
		} else {
			FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "10");
		}
		statBox.x += 110.0f / camera.scale.y;
		if (Button::offset.colliders)
			FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "Both");
		else {
			FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "Sprite");
		}
		return 0;
	}
}// namespace Label
