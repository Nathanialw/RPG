#pragma once

#include <string>
#include <vector>
#include "mouse_control.h"
#include "ui_frames.h"

namespace Game_Menu_Control {

  void Open_build_Menu() {
    //one large Rect with squares with the pic of buildings that are available
  }

  bool Check_Menu_Button() {
    //check if it is in menu frame
    if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.background.frame) || UI_Frames::topFrame.open) {
      for (int i = 0; i < UI_Frames::topFrame.buttons.size(); i++) {
        if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[0].button.frame)) {
          Utilities::Log(UI_Frames::topFrame.buttons[0].button.text);
          //open build menuc
          UI_Frames::topFrame.currentTab = UI_Frames::build;
          UI_Frames::topFrame.open = true;
          return true;
        } else if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[1].button.frame)) {
          Utilities::Log(UI_Frames::topFrame.buttons[1].button.text);
          UI_Frames::topFrame.currentTab = UI_Frames::train;
          UI_Frames::topFrame.open = true;
          return true;
        } else if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[2].button.frame)) {
          Utilities::Log(UI_Frames::topFrame.buttons[2].button.text);
          UI_Frames::topFrame.currentTab = UI_Frames::army;
          UI_Frames::topFrame.open = true;
          return true;
        } else if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[3].button.frame)) {
          Utilities::Log(UI_Frames::topFrame.buttons[3].button.text);
          UI_Frames::topFrame.currentTab = UI_Frames::serfs;
          UI_Frames::topFrame.open = true;
          return true;
        }
        if (!Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.submenu.frame)) {
          UI_Frames::topFrame.currentTab = UI_Frames::SIZE;
          UI_Frames::topFrame.open = false;
          return false;
        }
      }
    }

    if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.submenu.frame) || UI_Frames::topFrame.open) {
      Utilities::Log("inside submenu");
//      place full size texture on mouse

//      while the texture is on the mouse when you click the mouse it creates  the building
    }
    return false;
  }

//  set down and create building
  void Create_Building() {

  }
}