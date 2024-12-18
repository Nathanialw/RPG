#pragma once

#include "Input/mouse_control.h"
#include "action_components.h"
#include "ui_frames.h"
#include <string>
#include <vector>

namespace Game_Menu_Control {

  bool Select_Building(entt::registry &zone, int &state, UI_Frames::Menu_Frame &menu) {
    //    SDL_RenderCopyF(Graphics::renderer, menu.buttons[0].button.backgroundTexture, nullptr, &menu.submenu.frame);
    UI_Frames::Grid grid;
    for (int i = 0; i < menu.buttons[menu.currentTab].objects.size(); ++i) {
      SDL_FRect rect = {menu.submenu.frame.x + ((float) grid.Get_X() * menu.background.frame.h * 2.0f), menu.submenu.frame.y + ((float) grid.Get_Y() * menu.background.frame.h * 2.0f), (menu.background.frame.h * 2.0f), (menu.background.frame.h * 2.0f)};
      if (Mouse::FRect_inside_Screen_Cursor(rect)) {
        entt::entity entity;
        Action_Component::Action action;
        int index;
        float x;
        float y;
        menu.buttons[menu.currentTab].objects[i].build(zone, state, entity, action, index, x, y, menu.buttons[menu.currentTab].objects[i].name);
        return true;
      }
      grid.Update(4);
    }
    return false;
  }

  bool Close() {
      if (UI_Frames::topFrame.open) {
          UI_Frames::topFrame.currentTab = UI_Frames::SIZE;
          UI_Frames::topFrame.open = false;
          return true;
      }
      return false;
  }

  bool Check_Menu_Button(entt::registry &zone, int &state) {
    if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.background.frame)) {
      for (int i = 0; i < UI_Frames::topFrame.buttons.size(); i++) {
        if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[i].button.frame)) {
          UI_Frames::topFrame.currentTab = (UI_Frames::Menu_Tab) i;
          UI_Frames::topFrame.open = true;
          return true;
        }
      }
      return true;
    } else {
      if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.submenu.frame) && UI_Frames::topFrame.open) {
        if (Select_Building(zone, state, UI_Frames::topFrame)) {
          return true;
        }
        return true;
      } else {
        if (Mouse_Struct::mouseData.type == Component::Icon_Type::none) {
          Close();
          return false;
        }
      }
    }
    return false;
  }

  bool Check_Menu_Frame() {
    if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.background.frame)) {
      return true;
    }
    if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.submenu.frame) && UI_Frames::topFrame.open) {
      return true;
    }
    return false;
  }
}// namespace Game_Menu_Control