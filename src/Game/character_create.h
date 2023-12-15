#pragma once
#include "SDL2/SDL.h"
#include "SDL_FontCache/SDL_FontCache.h"
#include "base_structs.h"
#include "camera.h"
#include "character_create.h"
#include "close.h"
#include "events.h"
#include "graphics.h"
#include "menu.h"
#include "pause.h"
#include "timer.h"
#include "ui_elements.h"
#include "utilities.h"
#include "video.h"

namespace Character_Create {

  struct Size {
    i2 x, y;
    i2 w, h;
  };

  const char *file = "assets/videos/main_menu.ogv";

  void Background_Image() {
    SDL_RenderClear(Graphics::renderer);
  }

  SDL_Color colors[3] = {{255, 255, 255},
                         {255, 0, 0},
                         {55, 55, 55}};

  enum state {
    unselected,
    selected,
    disable,
    is_disabled
  };

  struct Button {
    SDL_FRect size = {};
    SDL_FRect scaledSize = {};
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *backgroundTexture = nullptr;
    SDL_Texture *textTexture = nullptr;
    const char *text = nullptr;
    state selected = unselected;
    bool button = false;
    SDL_Color color = {255,255,255};
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
    SDL_RenderPresent(Graphics::renderer);
  }

  enum Type {
    vertical,
    horizontal,
    image
  };

  void Build_Menu(Menu &menu, float x, float y, Type type) {
    //        set first index position
    if (menu.buttons[0].button) {
      menu.buttons[0].size = UI::Center_Rect(menu.buttons[0].textSurface->clip_rect);
      menu.buttons[0].size.x = (menu.buttons[0].size.x / (1.0f / x));
      menu.buttons[0].size.y = (menu.buttons[0].size.y / (1.0f / y));
      menu.buttons[0].size.w = menu.buttons[0].textSurface->clip_rect.h;
    }
    else if (menu.buttons[0].textSurface) {
      menu.buttons[0].size = UI::Center_Rect(menu.buttons[0].textSurface->clip_rect);
      menu.buttons[0].size.x = (menu.buttons[0].size.x / (1.0f / x));
      menu.buttons[0].size.y = (menu.buttons[0].size.y / (1.0f / y));
    }
    //        offset rest from first index
    if (type == vertical) {
      for (int i = 1; i < menu.buttons.size(); i++) {
        if (menu.buttons[i].button) {
          menu.buttons[i].size = UI::Center_Rect(menu.buttons[i].textSurface->clip_rect);
          menu.buttons[i].size.x = menu.buttons[i - 1].size.y + menu.buttons[i - 1].size.h + menu.spacing;
          menu.buttons[i].size.y = menu.buttons[i - 1].size.x;
          menu.buttons[i].size.w = menu.buttons[i - 1].textSurface->clip_rect.h;
        }
        else if (menu.buttons[i].textSurface) {
          menu.buttons[i].size = UI::Center_Rect(menu.buttons[i].textSurface->clip_rect);
          menu.buttons[i].size.y = menu.buttons[i - 1].size.y + menu.buttons[i - 1].size.h + menu.spacing;
          menu.buttons[i].size.x = menu.buttons[i - 1].size.x;
        }
      }
    } else if (type == horizontal) {
      for (int i = 1; i < menu.buttons.size(); i++) {
        if (menu.buttons[i].button) {
          menu.buttons[i].size = UI::Center_Rect(menu.buttons[i].textSurface->clip_rect);
          menu.buttons[i].size.x = menu.buttons[i - 1].size.x + menu.buttons[i - 1].size.w + menu.spacing;
          menu.buttons[i].size.y = menu.buttons[i - 1].size.y;
          menu.buttons[i].size.w = menu.buttons[i - 1].textSurface->clip_rect.h;
        }
        else if (menu.buttons[i].textSurface) {
          menu.buttons[i].size = UI::Center_Rect(menu.buttons[i].textSurface->clip_rect);
          menu.buttons[i].size.x = menu.buttons[i - 1].size.x + menu.buttons[i - 1].size.w + menu.spacing;
          menu.buttons[i].size.y = menu.buttons[i - 1].size.y;
        }
      }
    }
    else if (type == image) {
      for (auto &image : menu.buttons) {
        image.size = UI::Center_Rect({0, 0, 700, 700});
      }
    }
  }

  Menu Create_Menu(std::vector<const char *> labels, int index, float x, float y, Type type) {
    Menu menus;
    for (int i = 0; i < labels.size(); i++) {
      if (labels[i]) {
        Button button;
        menus.buttons.emplace_back(button);

        if (i == index) {
          menus.buttons[i].backgroundTexture = Graphics::default_icon;
        }
        if (-1 == index) {
          if (i == 0 || i == 2) {
            menus.buttons[i].backgroundTexture = Graphics::default_icon;
            menus.buttons[i].text = labels[i];
            menus.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, labels[i], colors[0]);
            menus.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menus.buttons[i].textSurface);
            menus.buttons[i].button = true;
          } else {
            menus.buttons[i].text = labels[i];
            menus.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, labels[i], colors[0]);
            menus.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menus.buttons[i].textSurface);
          }
        } else {
          menus.buttons[i].text = labels[i];
          menus.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, labels[i], colors[0]);
          menus.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menus.buttons[i].textSurface);
        }
      } else {
        Button button;
        menus.buttons.emplace_back(button);

//          menus.buttons[i].backgroundTexture = Graphics::default_icon;
          menus.buttons[i].size = {0, 0, 256, 512};
      }
    }

    Build_Menu(menus, x, y, type);
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

  void Menu_Render(Menu &menu) {
    for (int i = 0; i < menu.buttons.size(); i++) {
      if (Mouse::FRect_inside_Screen_Cursor(menu.buttons[i].size)) {
        if (menu.buttons[i].selected == unselected) {
          menu.buttons[i].selected = selected;
          SDL_FreeSurface(menu.buttons[i].textSurface);
          menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[1]);
          menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
        }
      } else if (menu.buttons[i].selected == selected) {
        menu.buttons[i].selected = unselected;
        SDL_FreeSurface(menu.buttons[i].textSurface);
        menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[0]);
        menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
      } else if (menu.buttons[i].selected == disable) {
        menu.buttons[i].selected = is_disabled;
        SDL_FreeSurface(menu.buttons[i].textSurface);
        menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[2]);
        menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
      }

      Graphics::Render_FRect(menu.buttons[i].backgroundTexture, menu.buttons[i].color, nullptr, &menu.buttons[i].size);
      SDL_RenderCopyF(Graphics::renderer, menu.buttons[i].textTexture, nullptr, &menu.buttons[i].size);
    }
  }

  void Increment(int &index, int array) {
    index++;
    if (index >= array) {
      index = 0;
    }
  }

  void Decrement(int &index, int array) {
    index--;
    if (index < 0) {
      index = array - 1;
    }
  }

  void Update_Species_List(Menu &menu, Character_Options::Customization &options) {
    auto races = Character_Options::Get_Race(options.sex);
    for (int n = 0; n < menu.buttons.size(); n++) {
      if (races[n] == "none") {
        menu.buttons[n].selected = disable;
        menu.buttons[n].backgroundTexture = nullptr;
        if ((int)options.species == n) {
          options.species = (Character_Options::Species) 0;
          menu.buttons[(int)options.species].backgroundTexture = Graphics::default_icon;
        }
      } else {
        menu.buttons[n].selected = selected;
      }
    }
  }

  void Toggle_Menu(Menu &menu, Character_Options::Customization &options) {
    for (int n = 0; n < menu.buttons.size(); n++) {
      if (options.sex == Character_Options::Sex::female) {
        if (n == 1) {
          menu.buttons[n].selected = disable;
        }
        menu.buttons[n].backgroundTexture = nullptr;
      }
      else {
        if (n == 1) {
          menu.buttons[n].selected = selected;
        }
        else {
          menu.buttons[n].backgroundTexture = Graphics::default_icon;
        }
      }
    }
  }

  void Toggle_Binary_Menu(Menu &menu, Character_Options::Customization &options) {
    if (options.sex == Character_Options::Sex::female) {
      for (int n = 0; n < menu.buttons.size(); n++) {
        menu.buttons[n].selected = disable;
        menu.buttons[n].backgroundTexture = nullptr;
      }
    }
    else {
      menu.buttons[0].selected = selected;
      menu.buttons[1].selected = selected;
      menu.buttons[0].backgroundTexture = Graphics::default_icon;
      menu.buttons[1].backgroundTexture = nullptr;
    }
  }

  void Set_Image(Menu &menu, Character_Options::Customization &options) {
    //unit
    menu.buttons[0].backgroundTexture = Character_Options::genderImages[(int)options.sex].species[(int)options.species];

    //hair
    menu.buttons[1].backgroundTexture = Character_Options::genderImages[(int)options.sex].legs;
    menu.buttons[2].backgroundTexture = Character_Options::genderImages[(int)options.sex].chest;
    menu.buttons[3].backgroundTexture = Character_Options::genderImages[(int)options.sex].weapon;
    if(options.sex == Character_Options::Sex::male) {
      //beard
      menu.buttons[4].backgroundTexture = Character_Options::genderImages[(int) options.sex].beardStyles[(int) options.beard];
      menu.buttons[4].color = Character_Options::Color[options.hairColor];
      //horns
      menu.buttons[5].backgroundTexture = Character_Options::genderImages[(int) options.sex].horns[options.horns];
    }
    else {
      menu.buttons[4].backgroundTexture = NULL;
      menu.buttons[5].backgroundTexture = NULL;
    }
    menu.buttons[6].backgroundTexture = Character_Options::genderImages[(int)options.sex].hairStyles[(int)options.hairStyle];
    menu.buttons[6].color = Character_Options::Color[options.hairColor];
  }

  void Menus_Events(std::vector<Menu> &menus, Character_Options::Customization &options) {
    while (SDL_PollEvent(&Events::event)) {
      switch (Events::event.type) {
        case SDL_QUIT: {
          for (int i = 0; i < menus[0].buttons.size(); i++) {
            SDL_FreeSurface(menus[0].buttons[i].textSurface);
          }
          Toggle();
          return;
        }

        case SDL_MOUSEBUTTONDOWN: {
          //Menu
          for (int j = 0; j < menus[0].buttons.size(); j++) {
            if (Mouse::FRect_inside_Screen_Cursor(menus[0].buttons[0].size)) {
              //go to game
              Toggle();
              return;
            } else if (Mouse::FRect_inside_Screen_Cursor(menus[0].buttons[1].size)) {
              //quit
              options.success = false;
              return;
            }
          }
          //Horns
          for (int k = 0; k < menus[1].buttons.size(); k++) {
            for (int m = 0; m < menus[1].buttons.size(); m++) {
              if (menus[1].buttons[m].selected != is_disabled) {
                if (Mouse::FRect_inside_Screen_Cursor(menus[1].buttons[0].size)) {
                  Increment(options.horns, Character_Options::genderImages[(int)options.sex].horns.size());
                  menus[1].buttons[1].backgroundTexture = nullptr;
                  menus[1].buttons[0].backgroundTexture = Graphics::default_icon;
                  Set_Image(menus[7], options);
                  return;
                }
                if (Mouse::FRect_inside_Screen_Cursor(menus[1].buttons[1].size)) {
                  Decrement(options.horns, Character_Options::genderImages[(int)options.sex].horns.size());
                  menus[1].buttons[1].backgroundTexture = Graphics::default_icon;
                  menus[1].buttons[0].backgroundTexture = nullptr;
                  Set_Image(menus[7], options);
                  return;
                }
              }
            }
          }
          //Beard Style
          for (int k = 0; k < menus[2].buttons.size(); k++) {
            if (menus[2].buttons[k].selected != is_disabled) {
              if (Mouse::FRect_inside_Screen_Cursor(menus[2].buttons[0].size)) {
                Decrement(options.beard, Character_Options::Get_facialHair(options.sex).size());
                Set_Image(menus[7], options);
                return;
              }
              if (Mouse::FRect_inside_Screen_Cursor(menus[2].buttons[2].size)) {
                Increment(options.beard, Character_Options::Get_facialHair(options.sex).size());
                Set_Image(menus[7], options);
                return;
              }
            }
          }
          //Hair Style
          for (int k = 0; k < menus[3].buttons.size(); k++) {
            if (Mouse::FRect_inside_Screen_Cursor(menus[3].buttons[0].size)) {
              Decrement(options.hairStyle, Character_Options::Get_Hair(options.sex).size());
              Set_Image(menus[7], options);
              return;
            }
            if (Mouse::FRect_inside_Screen_Cursor(menus[3].buttons[2].size)) {
              Increment(options.hairStyle, Character_Options::Get_Hair(options.sex).size());
              Set_Image(menus[7], options);
              return;
            }
          }
          //Hair Color
          for (int l = 0; l < menus[4].buttons.size(); l++) {
            if (Mouse::FRect_inside_Screen_Cursor(menus[4].buttons[0].size)) {
              Decrement(options.hairColor, Character_Options::Color.size());
              Utilities::Log(options.hairColor);
              Set_Image(menus[7], options);
              return;
            }
            if (Mouse::FRect_inside_Screen_Cursor(menus[4].buttons[2].size)) {
              Increment(options.hairColor, Character_Options::Color.size());
              Utilities::Log(options.hairColor);
              Set_Image(menus[7], options);
              return;
            }
          }
          //Gender
          for (int m = 0; m < menus[5].buttons.size(); m++) {
            if (Mouse::FRect_inside_Screen_Cursor(menus[5].buttons[0].size)) {
              options.sex = Character_Options::Sex::male;
              menus[5].buttons[1].backgroundTexture = nullptr;
              menus[5].buttons[0].backgroundTexture = Graphics::default_icon;
              options.hairStyle = 0;
              //enable horns
              options.horns = 0;
              Toggle_Binary_Menu(menus[1], options);
              //enable beard
              options.beard = 0;
              Toggle_Menu(menus[2], options);
              Update_Species_List(menus[6], options);
              Set_Image(menus[7], options);
              return;
            }
            if (Mouse::FRect_inside_Screen_Cursor(menus[5].buttons[1].size)) {
              options.sex = Character_Options::Sex::female;
              menus[5].buttons[1].backgroundTexture = Graphics::default_icon;
              menus[5].buttons[0].backgroundTexture = nullptr;
              options.hairStyle = 0;
              options.beard = 0;
              //disable horns
              options.horns = 0;
              Toggle_Binary_Menu(menus[1], options);
              //disable beard
              Toggle_Menu(menus[2], options);
              Update_Species_List(menus[6], options);
              Set_Image(menus[7], options);
              return;
            }
          }
          //Species
          for (int m = 0; m < menus[6].buttons.size(); m++) {
            if (Mouse::FRect_inside_Screen_Cursor(menus[6].buttons[m].size)) {
              if (menus[6].buttons[m].selected != is_disabled) {
                //clear all selected, quite hacky
                for (int n = 0; n < menus[6].buttons.size(); n++) {
                  menus[6].buttons[n].backgroundTexture = nullptr;
                }

                options.species = (Character_Options::Species) m;
                menus[6].buttons[(int)options.species].backgroundTexture = Graphics::default_icon;
                Set_Image(menus[7], options);
                return;
              }
            }
          }
          //
          break;
        }

        case SDL_KEYDOWN: {
          if (Events::event.key.keysym.sym == SDLK_ESCAPE) {
            Toggle();
            options.success = false;
            return;
          }
          break;
        }
      }
    }
  }

  Character_Options::Customization Customize(Character_Options::Customization &options) {
    Character_Options::Load_Start_Character_Images();
    std::vector<Menu> menus;

    menus.emplace_back(Create_Menu({"Start", "Exit"}, -2, 1.0f, 1.75f, horizontal));
    menus.emplace_back(Create_Menu({"None", "Horns"}, 0, 0.1f, 1.45f, horizontal));
    menus.emplace_back(Create_Menu({" ", "Beard Style", " "}, -1, 0.1f, 1.55f, horizontal));
    menus.emplace_back(Create_Menu({" ", "Hair Style", " "}, -1, 0.1f, 1.65f, horizontal));
    menus.emplace_back(Create_Menu({" ", "Hair Color", " "}, -1, 0.1f, 1.75f, horizontal));
    menus.emplace_back(Create_Menu({"male", "female"}, (int)options.sex, 0.1f, 1.85f, horizontal));
    menus.emplace_back(Create_Menu({"orc", "Zombie", "skeleton", "demon", "fleshbeast", "elves", "euro", "asian", "indian", "african"}, (int)options.species, 0.1f, 0.1f, vertical));
    menus.emplace_back(Create_Menu({NULL, NULL, NULL, NULL, NULL, NULL, NULL}, 0, 1.0f, 0.50f, image));

    toggleMenu = true;
    Set_Image(menus[7], options);

    while (toggleMenu) {
      Background_Image();
      Update_Cursor();

      if (toggleMenu) {
        for (auto &menu: menus) {
          Menu_Render(menu);
        }
        Menus_Events(menus, options);
      }

      Display_Mouse();
      if (!options.success) {
        return options;
      };
    }
    return options;
  }
}// namespace Character_Create
