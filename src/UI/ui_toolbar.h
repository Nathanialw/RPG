//
// Created by nathanial on 12/16/24.
//
#pragma  once
#include "SDL2/SDL.h"
#include "array"
#include "graphics.h"
#include "utilities.h"
#include "ui_elements.h"
#include "icons.h"
#include "textures.h"
#include "ui_minimap.h"

namespace  UI_toolbar {
    /*
        Make  a  toolbar  above the spell buttons with buttons to open
            -stats
            -inventory
            -low power spells
            -mid power spells
            -high power spells
            -general skills
            -mage skills
            -rogue skills
            -warrior skills
            -race/class perks
            -diety relations
            -events
            -minimap
            -prayers


            -stealth mode
            -selective targets
    */

    struct {
        float iconSize = 32.0f;
        std::array<SDL_FRect , 14> button;
        SDL_FRect frame = {0.0f, 0, 14 * iconSize, iconSize};
        SDL_FRect scaledFrame{};
        SDL_Texture *texture = nullptr; //render to this texture

    private:
        std::string buttonOn = "buttonon";
        std::string buttonOff = "buttonoff";

        std::string icon[14] = {
                "inventory",
                "character",
                "lowpower",
                "midpower",
                "highpower",
                "generalskills",
                "mageskills",
                "rogueskills",
                "warriorskills",
                "perks", //racial/class
                "stats", //dieties
                "events",
                "map",
                "religion" //prayers
        };

    public:
        SDL_Rect Icon_Clip_Rect(int i, bool on = false) {
            if (!on)
                return Icons::iconClipRects[icon[i] + buttonOff].clipRect;
            return Icons::iconClipRects[icon[i] + buttonOn].clipRect;
        }


    } toolBar;

    void Update_Position(SDL_FRect &frameRect) {
        SDL_Rect rect = Utilities::SDL_FRect_To_SDL_Rect(frameRect);
        frameRect = UI::Center_Rect(rect);
        int w;
        int h;
        SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
        frameRect.y = (float) h - frameRect.h - 64.0f;
    }

    void Update(f2 scale) {
        Update_Position(toolBar.frame);
        toolBar.scaledFrame = UI::Update_Scale(scale, toolBar.frame);

        if (!toolBar.texture) {
            toolBar.texture = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, toolBar.frame.w, toolBar.frame.h);
            SDL_SetRenderTarget(Graphics::renderer, toolBar.texture);

            for (int i = 0; i < toolBar.button.size(); i++) {
                SDL_FRect renderRect = {0.0f + (toolBar.frame.h * i), 0.0f, toolBar.frame.h, toolBar.frame.h};
                SDL_Rect clipRect = toolBar.Icon_Clip_Rect(i);
                SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &renderRect);
            }

            SDL_SetRenderTarget(Graphics::renderer, nullptr);
        }
    }

    bool Mouse_inside() {
        if (Mouse::bRect_inside_Cursor(toolBar.scaledFrame)) {
            return true;
        }
        return false;
    };

    int Get_Index(SDL_FRect &rect) {
        float mousePos = Mouse::screenMousePoint.x  - rect.x;
        if (mousePos < 0 || mousePos >= rect.w)
            return -1;

        return mousePos / rect.h;
    }

    void Highlight() {

        int i = Get_Index(toolBar.scaledFrame);
        if (i == -1)
            return;

        SDL_Rect clipRect = toolBar.Icon_Clip_Rect(i, true);
        SDL_FRect rect = {toolBar.scaledFrame.x + (toolBar.scaledFrame.h * i), toolBar.scaledFrame.y, toolBar.scaledFrame.h, toolBar.scaledFrame.h};

        SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &rect);
    }

    bool Click_Button() {
        int i = Get_Index(toolBar.scaledFrame);
        if (i == -1)
            return false;

        switch (i) {
            case 0:
                return true;
            case 1:
                return true;
            case 2:
                return true;
            case 3:
                return true;
            case 4:
                return true;
            case 5:
                return true;
            case 6:
                return true;
            case 7:
                return true;
            case 8:
                return true;
            case 9:
                return true;
            case 10:
                return true;
            case 11:
                return true;
            case 12:
                Minimap::Toggle();
                return true;
            case 13:
                return true;
            default:
                return false;

        }
    }

    void Render() {
        //render background
//        SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &rect);

            SDL_RenderCopyF(Graphics::renderer, toolBar.texture, nullptr, &toolBar.scaledFrame);
    }




        //and array of SDL_FRects to check against the mouse
        //function to render the toolbar
        //fuction to highlight the hovered button



}