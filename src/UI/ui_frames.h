#pragma once

#include "building.h"
#include "graphics.h"
#include "menu.h"
#include "unit_frames.h"
#include <string>
#include <vector>


namespace UI_Frames {

  class Grid {
  private:
    int x = 0;
    int y = 0;

  public:
    void Update(int w) {
      x++;
      if ((x > 0) && x % w == 0) {
        y++;
        x = 0;
      }
    }

    int Get_X() {
      return x;
    };

    int Get_Y() {
      return y;
    };
  };

  enum Menu_Tab {
    build,
    train,
    army,
    serfs,
    SIZE
  };

  int Build(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    Build::Orc::House(zone, index);
    return 0;
  }

  struct Building {
    SDL_Texture *icon = Graphics::default_icon;
    Spells::castSpell build = Build;
  };

  struct Menu_Button {
    UI::Text_Frame button;
    std::vector<UI::Text_Frame> buildButton;
    std::array<Building, 13> buildings;
  };

  struct Menu_Frame {
    UI::Image_Frame background;
    UI::Image_Frame submenu;
    std::vector<Menu_Button> buttons;
    Menu_Tab currentTab = SIZE;
    bool open = false;
  };

  std::vector<std::string> tabs = {"Build", "Train", "Army", "Serfs"};
  Menu_Frame topFrame;

  void Load_Buildings() {
    for (auto &tab: topFrame.buttons) {
      for (auto &building: tab.buildings) {
        building = {Graphics::default_icon, Build};
      }
    }
  }

  void Update_Frame_Data(f2 &scale, std::string &text, UI::Text_Frame &frame) {
    if (frame.textTexture == NULL) {
      Unit_Frames::Populate_Frame(frame, text, {100, 255, 50});
    }
    Unit_Frames::Update_Frame_Text(scale, frame, text, 40.0f);
  }

  //set the background frame in a position with a size and evenly space the buttons within
  void Init_Button_Menu() {
    topFrame.background.frame = {1024.0f, 0.0f, 512.0f, 64.0f};
    topFrame.submenu.frame = {topFrame.background.frame.x, topFrame.background.frame.y + topFrame.background.frame.h, topFrame.background.frame.w, 512.0};
  }

  void Build_Buttons(f2 scale) {
    topFrame.background.frame = UI::Update_Scale(scale, topFrame.background.frame);
    topFrame.submenu.frame = UI::Update_Scale(scale, topFrame.submenu.frame);

    topFrame.buttons.resize(tabs.size());
    float xPos = topFrame.background.frame.x;
    for (int i = 0; i < tabs.size(); i++) {
      topFrame.buttons[i].button.backgroundTexture = Graphics::tooltipBackground;
      topFrame.buttons[i].button.frame = {
          xPos,
          topFrame.background.frame.y,
          topFrame.background.frame.w / topFrame.buttons.size(),
          topFrame.background.frame.h,
      };
      Update_Frame_Data(scale, tabs[i], topFrame.buttons[i].button);
      xPos += topFrame.background.frame.w / topFrame.buttons.size();
    }
    topFrame.buttons.shrink_to_fit();
  }

  void Init_Frames(f2 scale) {
    Init_Button_Menu();
    Build_Buttons(scale);
  }

  void Show_Submenu(Menu_Frame &menu) {
    SDL_RenderCopyF(Graphics::renderer, menu.buttons[0].button.backgroundTexture, NULL, &menu.submenu.frame);
    Grid grid;
    for (auto building: menu.buttons[menu.currentTab].buildings) {
      SDL_FRect rect = {menu.submenu.frame.x + ((float) grid.Get_X() * menu.background.frame.h * 2.0f), menu.submenu.frame.y + ((float) grid.Get_Y() * menu.background.frame.h * 2.0f), (menu.background.frame.h * 2.0f), (menu.background.frame.h * 2.0f)};
      SDL_RenderCopyF(Graphics::renderer, building.icon, NULL, &rect);
      grid.Update(4);
    }
  }

  void Show_Menu_Frame(entt::registry &zone, Component::Camera &camera) {
    Init_Button_Menu();
    Build_Buttons(camera.scale);

    for (auto &menu: topFrame.buttons) {
      Update_Frame_Data(camera.scale, menu.button.text, menu.button);
      SDL_RenderCopyF(Graphics::renderer, menu.button.backgroundTexture, NULL, &menu.button.frame);
      SDL_RenderCopyF(Graphics::renderer, menu.button.textTexture, NULL, &menu.button.textFrame);
    }

    //render submenu
    switch (topFrame.currentTab) {
      case build: {
        Show_Submenu(topFrame);
        break;
      }
      case train: {
        Show_Submenu(topFrame);
        break;
      }
      case army: {
        Show_Submenu(topFrame);
        break;
      }
      case serfs: {
        Show_Submenu(topFrame);
        break;
      }
      case SIZE: {
        break;
      }
    }
  }
}// namespace UI_Frames