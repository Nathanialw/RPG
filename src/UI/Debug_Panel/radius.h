#pragma once
#include "Collision/collision_load.h"
#include "SDL2/SDL.h"
#include "SQLite_insert.h"
#include "components.h"
#include "mouse_control.h"
#include "string"
#include "ui_elements.h"


namespace Button {

  typedef int (*debugPanel)(const Component::Camera &camera, SDL_FRect statBox, const FC_Scale &scale, const SDL_Color &color, const std::string &tag);
  typedef int (*debugButtons)(Component::Camera &camera, SDL_FRect panel, const std::string &name, float &x, float &y);

  struct {
    bool colliders = false;
    float increment = 1;

    std::string table = "building_exteriors";

    std::string r = "radius";

  } radius;


  int Radius(Component::Camera &camera, SDL_FRect panel, const std::string &name, float &x, float &y) {
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
      x -= radius.increment;
      SQLite_Insert::Update_DB(radius.table, radius.r, name, -radius.increment);
      return 1;
    }
    button = {draw.x + g, draw.y + d, a, b};// right
    if (Mouse::bRect_inside_Cursor(button)) {
      x += radius.increment;
      SQLite_Insert::Update_DB(radius.table, radius.r, name, radius.increment);
      return 1;
    }
    //    button = {draw.x + c, draw.y + c, b, a};//up
    //    if (Mouse::bRect_inside_Cursor(button)) {
    //      y += radius.increment;
    //      SQLite_Insert::Update_DB(radius.table, radius.y, name, radius.increment);
    //      return 1;
    //    }
    //    button = {draw.x + c, draw.y + f, b, a};// down
    //    if (Mouse::bRect_inside_Cursor(button)) {
    //      y += -radius.increment;
    //      SQLite_Insert::Update_DB(radius.table, radius.y, name, -radius.increment);
    //      return 1;
    //    }


    button = {draw.x + h, draw.y + d, j, a};//
    if (Mouse::bRect_inside_Cursor(button)) {
      (radius.increment == 1) ? radius.increment = 10 : radius.increment = 1;
      return 1;
    }
    //    button = {draw.x + i, draw.y + d, j, a};//
    //    if (Mouse::bRect_inside_Cursor(button)) {
    //      radius.colliders = !radius.colliders;
    //      return 1;
    //    }
    return 0;
  }


}// namespace Button

namespace Label {

  int Radius(const Component::Camera &camera, SDL_FRect statBox, const FC_Scale &scale, const SDL_Color &color, const std::string &tag) {
    FC_DrawScaleRightColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, tag.c_str());
    statBox.x += 60.0f / camera.scale.y;//45.0f // 325.0f //
    if (Button::radius.increment == 1) {
      FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "1");
    } else {
      FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "10");
    }
    //    statBox.x += 110.0f / camera.scale.y;
    //    FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "Both");

    return 0;
  }
}// namespace Label
