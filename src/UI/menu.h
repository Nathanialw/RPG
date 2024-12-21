#pragma once

#include "Input/camera.h"
#include "Input/mouse_control.h"
#include "Input/pause.h"
#include "SDL2/SDL.h"
#include "SDL_FontCache/SDL_FontCache.h"
#include "base_structs.h"
#include "events.h"
#include "menu_options.h"
#include "ui_actionbar.h"
#include "ui_elements.h"
#include "ui_spellbook.h"
#include <array>

namespace Menu {

  struct Size {
    i2 x, y;
    i2 w, h;
  };

//  SDL_Color colors[2] = {{255, 255, 255},
//                         {255, 0, 0}};

  struct Button {
    SDL_FRect size;
    SDL_FRect scaledSize;
    SDL_Surface *textSurface;
    SDL_Texture *backgroundTexture;
    SDL_Texture *textTexture;
    const char *text;
    bool selected = false;
  };

  struct Menu {
    SDL_Texture *background;
    Uint8 spacing = Mouse::cursorRadius * 2;
    std::array<Button, 6> buttons;
    int i = -1;
    bool showOptions = false;
  } menu;

  //    need the attricute otherwise it WILL be optimized out
  void Build_Menu(Menu &menus) {
    //        set first index position
    menu.buttons[0].size = UI::Center_Rect(menu.buttons[0].textSurface->clip_rect);
    menu.buttons[0].size.y /= 2.0f;
    //        offset rest from first index
    for (int i = 1; i < menu.buttons.size(); i++) {
      menu.buttons[i].size = UI::Center_Rect(menu.buttons[i].textSurface->clip_rect);
      menu.buttons[i].size.y = menu.buttons[i - 1].size.y + menu.buttons[i - 1].size.h + menu.spacing;
    }
  }

  void Create_Menu(Menu &menus) {
    Menu tempMenu;
    std::vector<const char *> labels = {"Continue", "New Game", "Save", "Options", "Debug",  "Exit"};

    for (int i = 0; i < labels.size(); i++) {
      Button button;
      button.text = labels[i];
      button.textSurface = TTF_RenderText_Solid(Graphics::font, labels[i], Color::white);
      button.textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, button.textSurface);
      tempMenu.buttons[i] = button;
    }

    //        set first index position
    tempMenu.buttons[0].size = UI::Center_Rect(tempMenu.buttons[0].textSurface->clip_rect);
    tempMenu.buttons[0].size.y /= 2.0f;
    //        offset rest from first index
    for (int i = 1; i < tempMenu.buttons.size(); i++) {
      tempMenu.buttons[i].size = UI::Center_Rect(tempMenu.buttons[i].textSurface->clip_rect);
      tempMenu.buttons[i].size.y = tempMenu.buttons[i - 1].size.y + tempMenu.buttons[i - 1].size.h + tempMenu.spacing;
      ;
    }
    menu = tempMenu;
  }


  void Init() {
    Create_Menu(menu);
  }

  bool toggleMenu = false;

  bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
      if (toggleType == Toggle_Type::get)
          return toggleMenu;

      if (toggleType == Toggle_Type::on) {
          toggleMenu = true;
          if (!Pause::paused)
              Pause::Toggle();

          return true;
      }
      if (toggleType == Toggle_Type::off) {
          if (Pause::paused)
              Pause::Toggle();

          toggleMenu = false;
          return false;
      }

      if (toggleMenu) {
          toggleMenu = false;
          Pause::Toggle();
          return false;
      }

      toggleMenu = true;
      if (!Pause::paused)
          Pause::Toggle();
      return true;
  }

  void Render_Menu_FC(Component::Camera &camera) {
    for (int i = 0; i < menu.buttons.size(); i++) {
      menu.buttons[i].scaledSize = UI::Update_Scale(camera.scale, menu.buttons[i].size);

      SDL_Color color = Color::white;
      FC_Scale scale = {1.0f / camera.scale.x, 1.0f / camera.scale.y};

      if (Mouse::FRect_inside_Screen_Cursor(menu.buttons[i].scaledSize)) {
        if (!menu.buttons[i].selected) {
          menu.buttons[i].selected = 1;
          color = Color::red;
        }

      } else {
        if (menu.buttons[i].selected) {
          menu.buttons[i].selected = 0;
          color = Color::white;
        }
      }
      FC_DrawScale_Center(Graphics::fcFont, Graphics::renderer, menu.buttons[i].size.x, menu.buttons[i].size.y, scale, color, menu.buttons[i].text);
    }
  }

  void Render_Menu(Component::Camera &camera) {
    for (int i = 0; i < menu.buttons.size(); i++) {
      menu.buttons[i].scaledSize = UI::Update_Scale(camera.scale, menu.buttons[i].size);

      if (Mouse::FRect_inside_Screen_Cursor(menu.buttons[i].scaledSize)) {
        if (!menu.buttons[i].selected) {
          menu.buttons[i].selected = 1;
          SDL_FreeSurface(menu.buttons[i].textSurface);
          menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, Color::red);
          menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
        }

      } else {
        if (menu.buttons[i].selected) {
          menu.buttons[i].selected = 0;
          SDL_FreeSurface(menu.buttons[i].textSurface);
          menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, Color::white);
          menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
        }
      }
      SDL_RenderCopyF(Graphics::renderer, menu.buttons[i].textTexture, NULL, &menu.buttons[i].scaledSize);
    }
  }

    bool closeMenu = false;

    bool Close_Menu() {
        // Simulate key down
        Events::event.type = SDL_KEYDOWN;
        Events::event.key.keysym.sym = SDLK_ESCAPE;
        Events::event.key.state = SDL_PRESSED;
        SDL_PushEvent(&Events::event);

        // Simulate key up
        Events::event.type = SDL_KEYUP;
        Events::event.key.keysym.sym = SDLK_ESCAPE;
        Events::event.key.state = SDL_RELEASED;
        SDL_PushEvent(&Events::event);
        closeMenu = true;
    }

    bool Is_Menu_Open() {
        if (toggleMenu && closeMenu) {
            closeMenu = false;
            return false;
        }

        return toggleMenu;
    }

  int Show_Menu(Menu &menus, entt::registry &zone, int &state, Component::Camera &camera) {
    //    Debug::settings[Debug::Settings::fontRenderFC] ? Render_Menu(camera) : Render_Menu_FC(camera);
    //    Debug::settings[Debug::Settings::fontRenderFC] ? Render_Menu_FC(camera) : Render_Menu(camera);
    Render_Menu(camera);

    while (SDL_PollEvent(&Events::event)) {
      switch (Events::event.type) {

        case SDL_WINDOWEVENT: {
          if (Events::event.window.event == SDL_WINDOWEVENT_RESIZED) {
            //            recenter camera on player
            UI_Spellbook::Update_Position();
            Action_Bar::Update_Position(Action_Bar::actionBar.actionBar.actionBarFrame);
            Build_Menu(menu);
          }
          return -1;
        }

        case SDL_QUIT: {
          for (int i = 0; i < menu.buttons.size(); i++) {
            SDL_FreeSurface(menu.buttons[i].textSurface);
          }
          return 1;
        }

        case SDL_MOUSEBUTTONDOWN: {
          for (int j = 0; j < menu.buttons.size(); j++) {
            if (Mouse::FRect_inside_Screen_Cursor(menu.buttons[j].scaledSize)) {
              //              returns the index of the array the mouse has clicked on
              return j;
            }
          }
          break;
        }

        case SDL_KEYDOWN: {
          if (Events::event.key.keysym.sym == SDLK_ESCAPE) {
                Close_Menu();
              break;
          }
          break;
        }
      }
    }
    return menu.buttons.size() + 1;
  }


  bool Render_Menu(entt::registry &zone, int &state, Component::Camera &camera) {
    //pause with no inout
    if (toggleMenu) {
      UI::Overlay(camera.scale);
      if (menu.i != 4) {
        menu.i = Show_Menu(menu, zone, state, camera);
      }
      switch (menu.i) {
        case 0: {
            Close_Menu();
            break;
        }
        case 1: {
          return false;
        }
        case 2: {
          return true;
        }
        case 3: {
          return true;
        }
        case 4: {
          if (!menu.showOptions) {
            menu.showOptions = true;
            Menu_Options::Load_Options();
          }
          menu.i = Menu_Options::Show_Menu_Options(camera);
          if (menu.i == -1) {
            menu.showOptions = false;
            Build_Menu(menu);
          }
          break;
        }
        case 5: {
          state = 0;
          break;
        }
      }
    }
    return true;
  }
}// namespace Menu
