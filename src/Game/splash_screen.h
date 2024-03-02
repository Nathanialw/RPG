#pragma once
#include "Game/Game_Start/character_create.h"
#include "Input/camera.h"
#include "Input/pause.h"
#include "SDL2/SDL.h"
#include "SDL_FontCache/SDL_FontCache.h"
#include "base_structs.h"
#include "close.h"
#include "events.h"
#include "graphics.h"
#include "menu.h"
#include "timer.h"
#include "ui_elements.h"
#include "utilities.h"
#include "video.h"

namespace Splash_Screen {

  struct Size {
    i2 x, y;
    i2 w, h;
  };

  const char *file = "assets/videos/main_menu.ogv";

  void Background_Video() {
    SDL_RenderClear(Graphics::renderer);
    Video::Run_Video(file, false);
    SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 75);
    SDL_RenderFillRectF(Graphics::renderer, &Graphics::resolution);
  }

  void Background_Image() {
    SDL_RenderClear(Graphics::renderer);
  }

  SDL_Color colors[2] = {{255, 255, 255},
                         {255, 0, 0}};

  struct Button {
    SDL_FRect size = {};
    SDL_FRect scaledSize = {};
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *backgroundTexture = nullptr;
    SDL_Texture *textTexture = nullptr;
    const char *text = nullptr;
    bool selected = false;
  };

  struct Menu {
    SDL_Texture *background = nullptr;
    float spacing = Mouse::cursorRadius * 2;
    std::vector<Button> buttons;
  };

  SDL_FPoint Convert_FPoint_To_Scale(SDL_FPoint &rect) {

    SDL_FPoint fRenderToScreen = {
        float(rect.x),
        float(rect.y)};

    return fRenderToScreen;
  }

  void Update_Cursor() {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    Mouse::iXMouse = (float) mx;
    Mouse::iYMouse = (float) my;
    Mouse::iXWorld_Mouse = (Mouse::iXMouse);//getting mouse world Position corrected for scale
    Mouse::iYWorld_Mouse = (Mouse::iYMouse);//getting mouse world Position corrected for scale
    Mouse::iXMouse = Mouse::iXMouse;        // getting the screen mouse position corrected for scale
    Mouse::iYMouse = Mouse::iYMouse;        // getting the screen mouse position corrected for scale
    Mouse::mousePoint = {(float) mx, (float) my};
    Mouse::screenMousePoint = Convert_FPoint_To_Scale(Mouse::mousePoint);
  }

  void Display_Mouse() {
    SDL_Rect srcRect = {0, 0, 32, 32};
    SDL_FRect d = {Mouse::iXMouse, Mouse::iYMouse, 32.0f, 32.0f};

    SDL_RenderCopyF(Graphics::renderer, Graphics::cursor_0, &srcRect, &d);
  }

  void Build_Menu(Menu &menu, float x, float y) {
    //        set first index position
    menu.buttons[0].size = UI::Center_Rect(menu.buttons[0].textSurface->clip_rect);
    menu.buttons[0].size.x = (menu.buttons[0].size.x / (1.0f / x));
    menu.buttons[0].size.y = (menu.buttons[0].size.y / (1.0f / y));
    //        offset rest from first index
    for (int i = 1; i < menu.buttons.size(); i++) {
      menu.buttons[i].size = UI::Center_Rect(menu.buttons[i].textSurface->clip_rect);
      menu.buttons[i].size.y = menu.buttons[i - 1].size.y + menu.buttons[i - 1].size.h + menu.spacing;
      ;
    }
  }

  Menu Create_Menu(std::vector<const char *> labels) {
    Menu menus;
    for (int i = 0; i < labels.size(); i++) {
      Button button;
      menus.buttons.emplace_back(button);
      menus.buttons[i].text = labels[i];
      menus.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, labels[i], colors[0]);
      menus.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menus.buttons[i].textSurface);
    }

    Build_Menu(menus, 1.0f, 0.25f);
    return menus;
  }

  bool toggleMenu = true;

  void Toggle() {
    if (!toggleMenu) {
      toggleMenu = true;
    } else {
      toggleMenu = false;
    }
  }

  int Show_Menu(Menu &menu) {
    for (int i = 0; i < menu.buttons.size(); i++) {
      if (Mouse::FRect_inside_Screen_Cursor(menu.buttons[i].size)) {
        if (!menu.buttons[i].selected) {
          menu.buttons[i].selected = true;
          SDL_FreeSurface(menu.buttons[i].textSurface);
          menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[1]);
          menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
        }

      } else {
        if (menu.buttons[i].selected) {
          menu.buttons[i].selected = false;
          SDL_FreeSurface(menu.buttons[i].textSurface);
          menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[0]);
          menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
        }
      }
      SDL_RenderCopyF(Graphics::renderer, menu.buttons[i].textTexture, nullptr, &menu.buttons[i].size);
    }

    while (SDL_PollEvent(&Events::event)) {
      switch (Events::event.type) {
        case SDL_QUIT: {
          for (int i = 0; i < menu.buttons.size(); i++) {
            SDL_FreeSurface(menu.buttons[i].textSurface);
          }
          return 1;
        }

        case SDL_MOUSEBUTTONDOWN: {
          for (int j = 0; j < menu.buttons.size(); j++) {
            if (Mouse::FRect_inside_Screen_Cursor(menu.buttons[j].size)) {
              //                            returns the index of the array the mouse has clicked on
              return j;
            }
          }
          break;
        }

        case SDL_KEYDOWN: {
          switch (Events::event.key.keysym.sym) {
            case SDLK_ESCAPE: {
              Toggle();
              return 1;
            }
            case SDLK_RETURN:
              //return whichever is highlight and use arrow to move the highlighted up and down
              return 0;
            case SDLK_RETURN2:
              //return whichever is highlight and use arrow to move the highlighted up and down
              return 0;
          }
          break;
        }
      }
    }

    Display_Mouse();
    SDL_RenderPresent(Graphics::renderer);
    return 3;
  }

  bool Render_Menu(Menu &Menu) {
    if (toggleMenu) {
      Background_Video();
      int i = Show_Menu(Menu);
      if (i == 0) {
        Video::Run_Video(file, true);
        Toggle();
      }
      if (i == 1) {
        Video::Run_Video(file, true);
        Graphics::closeContext();
        return false;
      }
    }
    return true;
  }

  bool Main_Menu() {
    std::vector<const char *> Start_Menu = {"Create Character", "Exit"};
    Menu start_menu = Create_Menu(Start_Menu);
    while (toggleMenu) {
      Update_Cursor();
      if (!Render_Menu(start_menu)) {
        return false;
      };
    }
    return true;
  }
}// namespace Splash_Screen