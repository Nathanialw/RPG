//
// Created by nathanial on 12/19/24.
//
#pragma once
#include "graphics.h"
#include "Text/text.h"
#include "mouse_control.h"

namespace Tooltips {


    enum Screen_Position {
        SCREEN_TOP_LEFT,
        SCREEN_BOTTOM_LEFT,
        SCREEN_BOTTOM_RIGHT,
        SCREEN_TOP_RIGHT,
        MOUSE_TOP_LEFT,
        MOUSE_TOP_RIGHT,
        MOUSE_BOTTOM_LEFT,
        MOUSE_BOTTOM_RIGHT
    };

    template <size_t T>
    struct Properties {
        std::array<std::string, T> lines;
        std::array<float, T> spacing;
        float maxLineWidth = 550.0f;
        float x_offset = 0.0f;
        float y_offset = 0.0f;
        Screen_Position screenPosition = MOUSE_TOP_RIGHT;
    };

    SDL_FRect Position_Tooltip(f2 scale, float w, float h, float x_offset, float y_offset, Screen_Position screenPosition) {
        SDL_FRect rect{};
        w = w / scale.x;
        h = h / scale.y;
        x_offset = x_offset / scale.x;
        y_offset = y_offset / scale.y;

        switch (screenPosition) {
            case  Screen_Position::SCREEN_BOTTOM_RIGHT:
                rect = {Graphics::Screen.w - w - x_offset, Graphics::Screen.h - h - y_offset, w, h};
                break;
            case Screen_Position::SCREEN_TOP_LEFT:
                rect = {0.0f + x_offset, 0.0f + y_offset, w, h};
                break;
            case Screen_Position::SCREEN_BOTTOM_LEFT:
                rect = {0.0f + x_offset, Graphics::Screen.h - h - y_offset, w, h};
                break;
            case Screen_Position::SCREEN_TOP_RIGHT:
                rect = {Graphics::Screen.w - w - x_offset, 0.0f + y_offset, w, h};
                break;

            case Screen_Position::MOUSE_BOTTOM_RIGHT:
                rect = {Mouse::iXMouse + x_offset , Mouse::iYMouse + x_offset,w , h};
                break;
            case Screen_Position::MOUSE_TOP_LEFT:
                rect = {Mouse::iXMouse - w - x_offset, Mouse::iYMouse - h - y_offset, w, h};
                break;
            case Screen_Position::MOUSE_BOTTOM_LEFT:
                rect = {Mouse::iXMouse - w - x_offset, Mouse::iYMouse + y_offset, w, h};
                break;
            case Screen_Position::MOUSE_TOP_RIGHT:
                rect = {Mouse::iXMouse + x_offset, Mouse::iYMouse - h - y_offset, w, h};
                break;
        }

        return rect;
    }

//    void Create_Tooltip(const std::span<std::string> formattedDesc, const std::span<float> spacing, f2 scale, float x_offset, float y_offset, Screen_Position screenPosition, float maxLineWidth) {

    template <size_t T>
    void Create_Tooltip(Properties<T> properties) {
        auto dsd = Text::Get_Texture_Size(properties.lines, properties.spacing, properties.maxLineWidth);
        float w = dsd.x;
        float h = dsd.y;

        //render text at a hit scale (at least 2x) then render it to a texture, that will make it crisp
        float textScale = 2.0f;

        SDL_Texture * tooltipText = Graphics::Create_Canvas(w * textScale, h * textScale);
        SDL_SetRenderTarget(Graphics::renderer, tooltipText);
        SDL_RenderSetScale(Graphics::renderer, textScale, textScale);

        SDL_RenderCopyF(Graphics::renderer, Texture::tooltipBackground, nullptr, nullptr);
        Text::Texture_Data data = Text::Texture_Data{properties.lines, properties.spacing, properties.maxLineWidth};

        SDL_SetRenderTarget(Graphics::renderer, nullptr);

        SDL_FRect rect = Position_Tooltip(Graphics::Scale, w, h, properties.x_offset, properties.y_offset, properties.screenPosition);
        SDL_RenderCopyF(Graphics::renderer, tooltipText, nullptr, &rect);

        SDL_DestroyTexture(tooltipText);
    }








}