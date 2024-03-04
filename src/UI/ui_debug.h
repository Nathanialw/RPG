#pragma once
#include "Graphics/graphics.h"
#include "SDL2/SDL.h"
#include "SQLite_insert.h"
#include "components.h"
#include "entt/entt.hpp"
#include "mouse_control.h"
#include "string"
#include "tooltips.h"
#include "ui_elements.h"
#include "vector"

namespace UI_Debug {

  struct Unit_Info {
    SDL_FRect panelRect;
    SDL_Rect backgroundFrame = {0, 0, 464, 571};
    SDL_Texture *background = nullptr;
  } spellbook;

  struct Data {
    std::string name;
    Rendering_Components::Sprite_Offset offset;
  };

  void Update_Position() {
    spellbook.panelRect = UI::Left_Rect(spellbook.backgroundFrame);
    spellbook.background = Graphics::debug_window;
  }

  bool Mouse_Inside_Window(Component::Camera &camera) {
    SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
    if (Mouse::bRect_inside_Cursor(draw))
      return true;
    return false;
  }

  Data Get_Data(entt::registry &zone, entt::entity &entity) {
    auto name = zone.get<Component::Name>(entity).first;
    auto offset = zone.get<Rendering_Components::Sprite_Offset>(entity);
    return {name, offset};
  }

  SDL_FRect buttons[2];

  void Draw_Frame(Component::Camera &camera, SDL_FRect &draw, const Data &data) {
    float xSpacing = 32.0f;
    float ySpacing = 32.0f;
    float x = draw.x;
    float y = draw.y;
    float i = 1;
    float w = 0;
    SDL_FRect renderRect = {x + 192.0f, y + 16.0f, 0.0f, 0.0f};
    SDL_FRect statBox = Camera_Control::Convert_FRect_To_Scale(renderRect, camera);
    FC_Scale scale = {1.0f / camera.scale.x, 1.0f / camera.scale.y};
    SDL_Color color = {255, 255, 255, 255};
    SDL_Rect textRect = FC_DrawScaleRightColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, data.name.c_str());

    statBox.y += 20.0f / camera.scale.y;
    FC_DrawScaleRightColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "offsets");
    statBox.x += 60.0f / camera.scale.y;//45.0f // 325.0f //
    FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "revert");
    statBox.x += 110.0f / camera.scale.y;
    FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, "write");

    SDL_FRect textRectF = Utilities::SDL_Rect_To_SDL_FRect(textRect);
    if (Mouse::bRect_inside_Cursor(textRectF)) {
      //      SDL_FPoint mousePoint = {Mouse::iXMouse, Mouse::iYMouse};
      //      Tooltip::Tooltip_Text_Data tooltip = {spellbook.description[j], mousePoint};
      //      tooltip.charWidth = (10.0f / camera.scale.x);
      //      tooltip.charHeight = (20.0f / camera.scale.y);
      //      float xoffset = (mousePoint.x + (50.0f / camera.scale.x));
      //
      //      SDL_FRect tooltipBackground = {xoffset, mousePoint.y + tooltip.charHeight, (tooltip.text.size() * tooltip.charWidth), tooltip.charHeight};
      //      Tooltip::Render_Tooltip_Background(tooltipBackground, camera);
      //      SDL_Color color = {255, 255, 255, 255};
      //      FC_DrawScale_Center(Graphics::fcFont, Graphics::renderer, xoffset + ((tooltip.text.size() * tooltip.charWidth) / 2.0f), mousePoint.y, scale, color, spellbook.description[j].c_str());
    }
  }

  bool Update_Values(entt::registry &zone, Component::Camera &camera) {
    auto view = zone.view<Component::Selected, Component::Name, Rendering_Components::Sprite_Offset>();
    for (auto entity: view) {
      auto &offset = view.get<Rendering_Components::Sprite_Offset>(entity);
      SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
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

      button = {draw.x + c, draw.y + c, b, a};//up
      if (Mouse::bRect_inside_Cursor(button)) {
        offset.y++;
        return true;
      }
      button = {draw.x + c, draw.y + f, b, a};// down
      if (Mouse::bRect_inside_Cursor(button)) {
        offset.y--;
        return true;
      }
      button = {draw.x + e, draw.y + d, a, b};// left
      if (Mouse::bRect_inside_Cursor(button)) {
        offset.x++;
        return true;
      }
      button = {draw.x + g, draw.y + d, a, b};// right
      if (Mouse::bRect_inside_Cursor(button)) {
        offset.x--;
        return true;
      }
      button = {draw.x + h, draw.y + d, j, a};// revert
      if (Mouse::bRect_inside_Cursor(button)) {
        Utilities::Log("inside revert");
        return true;
      }
      button = {draw.x + i, draw.y + d, j, a};// write to DB
      if (Mouse::bRect_inside_Cursor(button)) {
        auto &name = view.get<Component::Name>(entity);
        SQLite_Insert::Update_Offset(name.first, offset.x, offset.y);
        Utilities::Log("inside write");
        return true;
      }
    }
    return false;
  }

  bool open;
  void Draw(entt::registry &zone, Component::Camera &camera) {
    open = false;
    if (Debug::settings[Debug::debugFrame]) {
      auto view = zone.view<Component::Selected>();
      for (auto entity: view) {
        open = true;
        auto data = Get_Data(zone, entity);
        SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
        SDL_RenderCopyF(Graphics::renderer, spellbook.background, &spellbook.backgroundFrame, &draw);
        Draw_Frame(camera, spellbook.panelRect, data);
      }
    }
  }
}// namespace UI_Debug