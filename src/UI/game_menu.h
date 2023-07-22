#pragma once
#include "graphics.h"
#include "ui_elements.h"
#include "unit_frames.h"
#include <string>
#include <vector>
#include "mouse_control.h"
#include "game_menu.h"
#include "ui_frames.h"
#include "utilities.h"

namespace Game_Menu_Control {










  void Open_build_Menu() {
    //one large Rect with squares with the pic of buildings that are available
  }

  
  bool Check_Menu_Button() {
    //check if it is in menu frame
    if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.background.frame)) {
      for (int i = 0; i < UI_Frames::topFrame.buttons.size(); i++) {
	if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[0].frame)) {
	  Utilities::Log(UI_Frames::topFrame.buttons[0].text);
	  //open build menu
	  
	  return true;
	}
	else if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[1].frame)) {
	  Utilities::Log(UI_Frames::topFrame.buttons[1].text);
	  return true;
	}
	else if (Mouse::FRect_inside_Screen_Cursor(UI_Frames::topFrame.buttons[2].frame)) {
	  Utilities::Log(UI_Frames::topFrame.buttons[2].text);
	  return true;
	}
      }
      return false;
    }
    

    
    
    //if it is chen which button it is hovering over

    //do something when clicked
  }
}
