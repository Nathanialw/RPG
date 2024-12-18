//
// Created by nathanial on 12/16/24.
//
#pragma once
#include "SDL2/SDL.h"
#include "array"
#include "icons.h"
#include "textures.h"

namespace  Button {

    template <size_t T>
    class Highlight {
        std::array<Uint8, T> highlight;
        Uint8 size = 0;

        void Remove(int value) {
            for (int j = 0; j < size; j++)
                if (highlight[j] == value) {
                    for (int k = j; k < size - 1; k++)
                        highlight[k] = highlight[k + 1];
                    size--;
                    return;
                }
        }

        void Add(int i) {
            highlight[size] = i;
            size++;
        }

        void Clear() {
            highlight.fill(-1);
            size = 0;
        }

    public:
        void Display(std::array<std::string, T>  icon, const std::string& state, SDL_FRect& scaledFrame) {
            for (int i = 0; i < size; i++) {
                SDL_Rect clipRect = Icons::iconClipRects[icon[highlight[i]] + state].clipRect;
                SDL_FRect rect = {scaledFrame.x + (scaledFrame.h * highlight[i]),scaledFrame.y, scaledFrame.h, scaledFrame.h};

                SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &rect);
            }
        }

        bool  Toggle(bool active, int i) {
            if (active)
                Add(i);
            else
                Remove(i);
            return true;
        }

        void Close_All() {
            Clear();
        }

        void Clear_Book() {
            for (int j = 0; j < size; j++)
                if (highlight[j]  != 0 && highlight[j] != 12) {
                    for (int k = j; k < size - 1; k++)
                        highlight[k] = highlight[k + 1];
                    size--;
                    return;
                }
        }
    };


}