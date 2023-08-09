#pragma once
#include "close.h"
#include "char_create.h"
#include "menu.h"
#include "timer.h"
#include "graphics.h"
#include "SDL2/SDL.h"
#include "SDL_FontCache/SDL_FontCache.h"
#include "base_structs.h"
#include "events.h"
#include "pause.h"
#include "camera.h"
#include "utilities.h"
#include "video.h"
#include "ui_elements.h"

namespace Main_Menu {

  struct Size {
    i2 x, y;
    i2 w, h;
  };

  const char *file = "assets/videos/main_menu.ogv";

  void Overlay() {
    SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 75);
    SDL_RenderFillRectF(Graphics::renderer, &Graphics::resolution);
  }


  SDL_Color colors[2] = {{255, 255, 255},
			 {255, 0,   0}};

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
    std::vector<Button> buttons;
  };

  //    stores menus, access with key
  Menu menus;

  SDL_FPoint Convert_FPoint_To_Scale(SDL_FPoint& rect) {

    SDL_FPoint fRenderToScreen = {
      float(rect.x),
      float(rect.y) };

    return fRenderToScreen;

  }

  void Update_Cursor() {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    Mouse::iXMouse = (float)mx;
    Mouse::iYMouse = (float)my;
    Mouse::iXWorld_Mouse = (Mouse::iXMouse);//getting mouse world Position corrected for scale
    Mouse::iYWorld_Mouse = (Mouse::iYMouse);//getting mouse world Position corrected for scale
    Mouse::iXMouse = Mouse::iXMouse;  // getting the screen mouse position corrected for scale
    Mouse::iYMouse = Mouse::iYMouse;  // getting the screen mouse position corrected for scale
    Mouse::mousePoint = { (float)mx, (float)my };
    Mouse::screenMousePoint = Convert_FPoint_To_Scale(Mouse::mousePoint);
  }

  void Display_Mouse() {
    SDL_Rect srcRect = { 0, 0 , 32, 32 };
    SDL_FRect d = { Mouse::iXMouse, Mouse::iYMouse, 32.0f, 32.0f };

    SDL_RenderCopyF(Graphics::renderer, Graphics::cursor_0, &srcRect, &d);
  }

  //    void Update_Scale(Menu &menu)
  ////    zone.emplace<Component::Camera>(entity, 0.0f, 0.0f, (Graphics::resolution.w), (Graphics::resolution.h), 2.0f, 2.0f);
  //    {
  //        //        set first index
  //        menu.buttons[0].size.x = (Graphics::resolution.w / 2) - (menu.buttons[0].textSurface->clip_rect.w / 2);
  //        menu.buttons[0].size.y = (Graphics::resolution.h / 4) - (menu.buttons[0].textSurface->clip_rect.h / 2);
  //        menu.buttons[0].size.w = menu.buttons[0].textSurface->clip_rect.w;
  //        menu.buttons[0].size.h = menu.buttons[0].textSurface->clip_rect.h;
  //
  //        //        offset rest
  //        for (int i = 1; i < menu.buttons.size(); i++)
  //        {
  //            menu.buttons[i].size.x = (Graphics::resolution.w / 2) - (menu.buttons[i].textSurface->clip_rect.w / 2);
  //            menu.buttons[i].size.y = menu.buttons[i-1].size.y + menu.buttons[i-1].size.h + menu.spacing;
  //            menu.buttons[i].size.w = menu.buttons[i].textSurface->clip_rect.w;
  //            menu.buttons[i].size.h = menu.buttons[i].textSurface->clip_rect.h;
  //        }
  //    }

  void Build_Menu(Menu &menu)
  {
    //        set first index position
    menu.buttons[0].size = UI::Center_Rect(menu.buttons[0].textSurface->clip_rect);
    menu.buttons[0].size.y /= 4.0f;
    //        offset rest from first index
    for (int i = 1; i < menu.buttons.size(); i++)
      {
	menu.buttons[i].size = UI::Center_Rect(menu.buttons[i].textSurface->clip_rect);
	menu.buttons[i].size.y = menu.buttons[i-1].size.y + menu.buttons[i-1].size.h + menu.spacing;;
      }
  }

  void Create_Menu() {

    std::vector<const char *> labels = {"Start", "Exit"};

    for (int i = 0; i < labels.size(); i++) {
      Button button;
      menus.buttons.emplace_back(button);
      menus.buttons[i].text = labels[i];
      menus.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, labels[i], colors[0]);
      menus.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menus.buttons[i].textSurface);
    }

    Build_Menu(menus);
  }


  bool toggleMenu = true;

  void Toggle() {
    if (!toggleMenu) {
      toggleMenu = true;
    } else {
      toggleMenu = false;
    }
  }

  int Show_Menu(entt::registry &zone)
  {
    SDL_RenderClear(Graphics::renderer);
    Video::Run_Video(file, false);


    Build_Menu(menus);
    Menu &menu = menus;

    for (int i = 0; i < menu.buttons.size(); i++)
      {
	//            SDL_FRect rectf;
	//            SDL_Rect rect1 = Utilities::SDL_FRect_To_SDL_Rect(menu.buttons[i].size);
	//            menu.buttons[i].scaledSize = UI::Center_Rect(rectf, rect1);

	if (Mouse::FRect_inside_Screen_Cursor( menu.buttons[i].size))
	  {
	    if (!menu.buttons[i].selected)
	      {
		menu.buttons[i].selected = 1;
		SDL_FreeSurface(menu.buttons[i].textSurface);
		menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[1]);
		menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
	      }

	  }
	else
	  {
	    if (menu.buttons[i].selected)
	      {
		menu.buttons[i].selected = 0;
		SDL_FreeSurface(menu.buttons[i].textSurface);
		menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[0]);
		menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
	      }
	  }
	SDL_RenderCopyF(Graphics::renderer, menu.buttons[i].textTexture, NULL, & menu.buttons[i].size);
      }


    while (SDL_PollEvent(&Events::event))
      {
	switch (Events::event.type)
	  {
	  case SDL_QUIT: {
	    for (int i = 0; i < menu.buttons.size(); i++)
	      {
		SDL_FreeSurface(menu.buttons[i].textSurface);
	      }
	    return 1;
	  }

	  case SDL_MOUSEBUTTONDOWN:
	    {
	      for (int j = 0; j < menu.buttons.size(); j++)
		{
		  if (Mouse::FRect_inside_Screen_Cursor( menu.buttons[j].size))
		    {
		      //                            returns the index of the array the mouse has clicked on
		      return j;
		    }
		}
	      break;
	    }

	  case SDL_KEYDOWN:
	    {
	      if (Events::event.key.keysym.sym == SDLK_ESCAPE)
		{
		  Toggle();
		  break;
		}
	      break;
	    }
	  }
      }

    Display_Mouse();
    SDL_RenderPresent(Graphics::renderer);
    return 3;
  }

  //    int Show_Menu(entt::registry &zone) {
  //        Menu &menu = menus["menu"];
  //
  //        for (int i = 0; i < menu.buttons.size(); i++)
  //        {
  //            if (Mouse::FRect_inside_Screen_Cursor(menu.buttons[i].size))
  //            {
  //                if (!menu.buttons[i].selected)
  //                {
  //                    menu.buttons[i].selected = 1;
  //                    SDL_FreeSurface(menu.buttons[i].textSurface);
  //                    Utilities::Log("----");
  //                    Utilities::Log(menu.buttons[i].textSurface);
  //                    menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[1]);
  //                    Utilities::Log(menu.buttons[i].textSurface);
  //                    menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
  //                }
  //            }
  //            else
  //            {
  //                if (menu.buttons[i].selected)
  //                {
  //                    menu.buttons[i].selected = 0;
  //                    SDL_FreeSurface(menu.buttons[i].textSurface);
  //                    menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[0]);
  //                    menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
  //                }
  //            }
  //        }
  //
  //        while (SDL_PollEvent(&Events::event)) {
  //            switch (Events::event.type) {
  //                case SDL_QUIT: {
  //                    for (int i = 0; i < menu.buttons.size(); i++) {
  //                        SDL_FreeSurface(menu.buttons[i].textSurface);
  //                    }
  //                    return 1;
  //                }
  //
  //                case SDL_MOUSEBUTTONDOWN: {
  //                    for (int j = 0; j < menu.buttons.size(); j++) {
  //                        if (Mouse::FRect_inside_Screen_Cursor(menu.buttons[j].size)) {
  ////                            returns the index of the array the mouse has clicked on
  //                            return j;
  //                        }
  //                    }
  //                    break;
  //                }
  //
  //                case SDL_KEYDOWN: {
  //                    if (Events::event.key.keysym.sym == SDLK_ESCAPE) {
  //                        Toggle();
  //                        break;
  //                    }
  //                    break;
  //                }
  //            }
  //        }
  //
  ////      render menu
  //        SDL_RenderClear(Graphics::renderer);
  //        Video::Run_Video(file, false);
  //        for (int i = 0; i < menu.buttons.size(); i++) {
  //            SDL_RenderCopyF(Graphics::renderer, menu.buttons[i].textTexture, NULL, &menu.buttons[i].size);
  //        }
  //        Display_Mouse();
  //        SDL_RenderPresent(Graphics::renderer);
  //        return 3;
  //    }

  void Render_Menu(entt::registry &zone) {
    if (toggleMenu) {
      Overlay();
      int i = Show_Menu(zone);
      if (i == 0) {
	Video::Run_Video(file, true);
	Toggle();
      }
      if (i == 1) {
	Video::Run_Video(file, true);
	Graphics::closeContext();
      }
    }
  }

  void Menu_Options() {

    Create_Menu();

    while (toggleMenu) {
      Update_Cursor();
      Render_Menu(World::zone);
    }
    Char_Create::Character_Selection();
  }

  //  BACKGROUND VIDEO



  //  MAYBE A GOOD PLACE TO TRY ADDING SOUNDS

}
