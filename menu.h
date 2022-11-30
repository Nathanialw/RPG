#include "timer.h"
#include "graphics.h"
#include "SDL2/SDL.h"
#include "SDL_FontCache.h"

namespace Menu {
    bool toggleMenu = false;

    void Overlay() {
        SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 75);
        SDL_RenderFillRectF(Graphics::renderer, &Graphics::resolution);
    }

    void Menu_List () {



     //   SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 200);
      //  SDL_FRect rect = {Graphics::resolution.x ,0, 200, 600};
      //  SDL_RenderFillRectF(Graphics::renderer, &rect);
    }

    void Toggle() {
        if (!toggleMenu) {
            toggleMenu = true;
            Timer::Pause_Control();
        }
        else {
            toggleMenu = false;
            Timer::Pause_Control();
        }
    }

    void Render_Menu() {
        if (toggleMenu) {
            Overlay();
            Menu_List();
        }
    }
}