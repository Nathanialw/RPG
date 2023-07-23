#pragma once
#include <string>
#include <vector>
#include "game_menu.h"
#include "mouse_control.h"
#include "ui_frames.h"

namespace Game_Menu_Control {

  void Open_build_Menu() {
    //one large Rect with squares with the pic of buildings that are available
  }

  
  bool Check_Menu_Button() {
    //check if it is in menu frame
     if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.background.frame)) {
       for (int i = 0; i < UI_Frames::topFrame.buttons.size(); i++) {
     	if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[0].button.frame)) {
     	  Utilities::Log(UI_Frames::topFrame.buttons[0].button.text);
     	  //open build menu
     	  UI_Frames::topFrame.currentTab = UI_Frames::build;
     	  return true;
     	}
     	else if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[1].button.frame)) {
     	  Utilities::Log(UI_Frames::topFrame.buttons[1].button.text);
     	  UI_Frames::topFrame.currentTab = UI_Frames::train;
     	  return true;
     	}
     	else if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[2].button.frame)) {
     	  Utilities::Log(UI_Frames::topFrame.buttons[2].button.text);
     	  UI_Frames::topFrame.currentTab = UI_Frames::army;
     	  return true;
     	}
     	else if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[3].button.frame)) {
     	  Utilities::Log(UI_Frames::topFrame.buttons[3].button.text);
     	  UI_Frames::topFrame.currentTab = UI_Frames::serfs;
     	  return true;
     	}
     	else if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[3].button.frame)) {
     	  Utilities::Log(UI_Frames::topFrame.buttons[3].button.text);
     	  UI_Frames::topFrame.currentTab = UI_Frames::SIZE;
     	  return true;
     	}
     	UI_Frames::topFrame.currentTab = UI_Frames::SIZE;
     	return false;
       }
       UI_Frames::topFrame.currentTab = UI_Frames::SIZE;
     }
    return false;
  }
    
  //if it is chen which button it is hovering over

  //do something when clicked

}
