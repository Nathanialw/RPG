#pragma once
#include "graphics.h"
#include "ui_elements.h"
#include "unit_frames.h"
#include <string>
#include <vector>

namespace UI_Frames {

  UI::Text_Frame menuButton;
  
  struct Menu_Frame {
    UI::Image_Frame background;
    std::vector<UI::Text_Frame> buttons = { menuButton, menuButton, menuButton };
  };

  std::vector<std::string> text = { "Build", "Train", "Army", "test3", };

  Menu_Frame topFrame;

  void Update_Frame_Data(f2 &scale, std::string &text, UI::Text_Frame &frame) {
    if (frame.textTexture == NULL) {
      Unit_Frames::Populate_Frame(frame, text, {100,255,50});
    }
    Unit_Frames::Update_Frame_Text(scale, frame, text, 40.0f);  
  }
  
  //set the background frame in a position with a size and evenly space the buttons within
  void Init_Button_Menu() {
    topFrame.background.frame = { 1024.0f, 0.0f, 512.0f, 64.0f };
  }

  void Build_Buttons(f2 scale) {
    topFrame.background.frame = UI::Update_Scale(scale, topFrame.background.frame);

    topFrame.buttons.resize(text.size());
    float xPos = topFrame.background.frame.x;
    for (int i = 0; i < text.size(); i++) {
      topFrame.buttons[i].backgroundTexture = Graphics::tooltipBackground;
      topFrame.buttons[i].frame = {
	xPos,
	topFrame.background.frame.y,
	topFrame.background.frame.w / topFrame.buttons.size(),
	topFrame.background.frame.h,
      };

      Update_Frame_Data(scale, text[i], topFrame.buttons[i]);
      xPos += topFrame.background.frame.w / topFrame.buttons.size();           
    }
    topFrame.buttons.shrink_to_fit();
  }

  void Init_Frames(f2 scale) {
    Init_Button_Menu();
    Build_Buttons(scale);
  }

  void Show_Menu_Frame(entt::registry &zone, Component::Camera &camera) {
    Init_Frames(camera.scale);
    topFrame.background.frame = UI::Update_Scale(camera.scale, topFrame.background.frame);      

    for (auto &button : topFrame.buttons) {
      SDL_RenderCopyF(Graphics::renderer, button.backgroundTexture, NULL, &button.frame);
      SDL_RenderCopyF(Graphics::renderer, button.textTexture, NULL, &button.textFrame);
    }
  }
}
