//
// Created by nathanial on 12/16/24.
//
#pragma once
#include "highlight.h"
#include "graphics.h"
#include "icons.h"

namespace  Button {

    void Update_Position(SDL_FRect &frameRect) {
        SDL_Rect rect = Utilities::SDL_FRect_To_SDL_Rect(frameRect);
        frameRect = UI::Center_Rect(rect);
        int w;
        int h;
        SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
        frameRect.y = (float) h - frameRect.h - 64.0f;
    }

    template <size_t T, typename Action>
    class Bar {
    private:
        float iconSize = 32.0f;
        std::array<SDL_FRect , T> button;
        SDL_FRect frame{};
        SDL_FRect scaledFrame{};
        SDL_Texture *texture = nullptr; //render to this texture

        std::array<Action, T> action;
        Highlight<T> highlight;

        std::string buttonOn;
        std::string buttonOff;

        std::array<std::string, T> icon;

        SDL_Rect Icon_Clip_Rect(int i, bool on = false) {
            if (!on)
                return Icons::iconClipRects[icon[i] + buttonOff].clipRect;

            return Icons::iconClipRects[icon[i] + buttonOn].clipRect;
        }

        int Get_Index(SDL_FRect &rect) {
            float mousePos = Mouse::screenMousePoint.x  - rect.x;
            if (mousePos < 0 || mousePos >= rect.w)
                return -1;

            return mousePos / rect.h;
        }

    public:
        Bar(std::array<std::pair<std::string, Action>, T> iconStrings,
                const std::string &active,
                const std::string &inactive,
                float iconWidth) :
                buttonOn(active),
                buttonOff(inactive),
                iconSize(iconWidth) {

            for (int i = 0; i < T; i++) {
                icon[i] = iconStrings[i].first;
                action[i] = iconStrings[i].second;
            }
            frame = {0.0f, 0.0f, T * iconSize, iconSize};
        }

        void Display() {
            SDL_RenderCopyF(Graphics::renderer, texture, nullptr, &scaledFrame);
            highlight.Display(icon, buttonOn, scaledFrame);
        }

        void Update(f2 scale) {
            Update_Position(frame);
            scaledFrame = UI::Update_Scale(scale, frame);

            if (!texture) {
                texture = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frame.w, frame.h);
                SDL_SetRenderTarget(Graphics::renderer, texture);

                for (int i = 0; i < T; i++) {
                    SDL_FRect renderRect = {0.0f + (frame.h * i), 0.0f, frame.h, frame.h};
                    SDL_Rect clipRect = Icon_Clip_Rect(i);
                    SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &renderRect);
                }

                SDL_SetRenderTarget(Graphics::renderer, nullptr);
            }
        }

        void Hover_Highlight() {
            int i = Get_Index(scaledFrame);
            if (i == -1)
                return;

            SDL_Rect clipRect = Icon_Clip_Rect(i, true);
            SDL_FRect rect = {scaledFrame.x + (scaledFrame.h * i), scaledFrame.y, scaledFrame.h, scaledFrame.h};

            SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &rect);
        }

        bool Mouse_Inside() {
            if (Mouse::bRect_inside_Cursor(scaledFrame))
                return true;

            return false;
        };


        bool Close_All() {
            bool wasOpen = false;

            highlight.Close_All();
            for (int i = 0; i < T; ++i) {
                if (action[i](Toggle_Type::get)) {
                    action[i](Toggle_Type::off);
                    wasOpen = true;
                }
            }

            return wasOpen;
        }

        void Close_Book(int n) {
            highlight.Clear_Book();

            for (int i = 0; i < T; ++i) {
                if (i == n || i == 0 || i == 12)
                    continue;
                action[i](Toggle_Type::off);
            }
        }

        void Toggle(int i) {
            if (i != 0  && i != 12)
                Close_Book((int)i);
            highlight.Toggle(action[i](Toggle_Type::toggle), i);
        }

        bool Click_Button() {
            int i = Get_Index(scaledFrame);
            if (i == -1)
                return false;

            if (i != 0 && i != 12)
                Close_Book(i);

            return highlight.Toggle(action[i](Toggle_Type::toggle), i);
        }

        int Get_Open() {
            for (int i = 0; i < T; ++i) {
                if (action[i](Toggle_Type::get))
		    if (i != 0 && i != 12)
                    	return i;
            }
            return -1;
        }
    };


}