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
    int w = 0;

  public:
    void Update(int w) {
      x++;
      if ((x > 0) && x % w == 0) {
        y++;
        x = 0;
      }
    }

    float Get_X() {
      return (float) x;
    };

    float Get_Y() {
      return (float) y;
    };
  };

  enum Menu_Tab {
    build,
    train,
    army,
    serfs,
    SIZE
  };

  int Build(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
    Build::Orc::House(zone, index, objectName);
    return 0;
  }

  struct Building {
    Component::Icon icon = {Graphics::default_icon};
    std::string name;
    Spells::castSpell build;
  };

  struct Menu_Button {
    UI::Text_Frame button;
    std::vector<Building> objects;
  };

  struct Menu_Frame {
    UI::Image_Frame background;
    UI::Image_Frame submenu;
    std::array<Menu_Button, Menu_Tab::SIZE> buttons;
    Menu_Tab currentTab = SIZE;
    bool open = false;
  };

  std::vector<std::string> tabs = {"Build", "Train", "Army", "Serfs"};
  Menu_Frame topFrame;

  const int numHouses = 63;
  void Load_Buildings() {
    std::array<std::string, numHouses> houses;
    //    houses =
    //        {"House 1/House 1 T1",
    //         "House 1/House 2 T1",
    //         "House 1/House 3 T1",
    //         "House 1/House 4 T1",
    //         "House 1/House 5 T1",
    //         "House 2/House 1 T1",
    //         "House 2/House 2 T1",
    //         "House 2/House 4 T1",
    //         "House 2/House 5 T1",
    //         "House 2/House 6 T1",
    //         "House 2/House 3 T1",
    //         "House 3/House 7 T1",
    //         "House 3/House 8 T1",
    //         "House 3/House 9 T1",
    //         "House 4/House 10 T1",
    //         "House 4/House 11 T1",
    //         "House 4/House 12 T1",
    //         "House 4/House 13 T1",
    //         "House 5/House 1 T1",
    //         "House 5/House 2 T1",
    //         "House 5/House 3 T1",
    //         "House 5/House 4 T1",
    //         "House 5/House 5 T1",
    //         "House 5/House 6 T1",
    //         "House 5/House 7 T1",
    //         "House 5/House 8 T1",
    //         "House/House 1 T1",
    //         "House/House 2 T1",
    //         "House/House 3 T1",
    //         "House/House 4 T1",
    //         "House/House 5 T1",
    //         "House/House 6 T1",
    //         "House/Shade T1",
    //         "Tower 1/Tower 1 T1",
    //         "Tower 1/Tower 2 T1",
    //         "Tower 1/Tower 3 T1",
    //         "Tower 1/Tower 4 T1"};

    houses = {"Building1_1"};
    Building building;
    for (auto house: houses) {
      building.name = house;
      building.icon.clipSprite = Icons::buildingIconClipRects[building.name].clipRect;
      building.icon.pTexture = Graphics::medievelBuildingIcons;
      building.build = Build;
      topFrame.buttons[Menu_Tab::build].objects.emplace_back(building);
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
    //x needs to be screen width -frame width
    //needs to update on screen resize
    topFrame.background.frame = {0.0f, 0.0f, 512.0f, 64.0f};
    topFrame.submenu.frame = {topFrame.background.frame.x, topFrame.background.frame.y + topFrame.background.frame.h, topFrame.background.frame.w, ((topFrame.background.frame.h * 2.0f) * (ceil(numHouses / 4.0f)))};
  }

  void Build_Buttons(Component::Camera &camera) {
    topFrame.background.frame = UI::Update_Scale(camera.scale, topFrame.background.frame);
    topFrame.submenu.frame = UI::Update_Scale(camera.scale, topFrame.submenu.frame);
    topFrame.background.frame.x = camera.screen.w - topFrame.background.frame.w;
    topFrame.submenu.frame.x = camera.screen.w - topFrame.submenu.frame.w;

    float xPos = topFrame.background.frame.x;
    for (int i = 0; i < tabs.size(); i++) {
      topFrame.buttons[i].button.backgroundTexture = Graphics::tooltipBackground;
      topFrame.buttons[i].button.frame = {
          xPos,
          topFrame.background.frame.y,
          topFrame.background.frame.w / topFrame.buttons.size(),
          topFrame.background.frame.h,
      };
      Update_Frame_Data(camera.scale, tabs[i], topFrame.buttons[i].button);
      xPos += topFrame.background.frame.w / topFrame.buttons.size();
    }
  }

  void Show_Submenu(Menu_Frame &menu) {
    SDL_RenderCopyF(Graphics::renderer, menu.buttons[0].button.backgroundTexture, NULL, &menu.submenu.frame);
    Grid grid;
    for (auto building: menu.buttons[menu.currentTab].objects) {
      SDL_FRect rect = {menu.submenu.frame.x + (grid.Get_X() * menu.background.frame.h * 2.0f), menu.submenu.frame.y + (grid.Get_Y() * menu.background.frame.h * 2.0f), (menu.background.frame.h * 2.0f), (menu.background.frame.h * 2.0f)};
      rect.x += (rect.w * 0.1f);
      rect.y += (rect.h * 0.1f);
      rect.w -= (rect.w * 0.1f);
      rect.w -= (rect.w * 0.1f);
      SDL_RenderCopyF(Graphics::renderer, building.icon.pTexture, &building.icon.clipSprite, &rect);
      grid.Update(4);
    }
  }

  void Show_Menu_Frame(entt::registry &zone, Component::Camera &camera) {
    Init_Button_Menu();
    Build_Buttons(camera);

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