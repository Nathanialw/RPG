//
// Created by nathanial on 12/19/24.
//
#pragma once
#include "graphics.h"

namespace Tooltips {

    class Tooltip {
        float w = -1;
        float h = -1;
        SDL_Texture *textTexture = nullptr;
        SDL_Texture *backgroundTexture = nullptr;

        std::vector<std::string> lines;

    public:
        Tooltip() = default;


    private:
        void AddText(std::array<Tooltip::Tooltip_Render_Data, rows> &renderArray, int &renderArrayIndex, Tooltip::Tooltip_Text_Data &tooltip, SDL_Color color, std::string line = " ") {
            tooltip.text = line;
            renderArray[renderArrayIndex] = Create_Text(tooltip, color, rows);
            renderArrayIndex++;
        }

        void Create_Tooltip_Text(std::string text, SDL_Color color) {
            //parse the text and create a texture


        }

    public:

        void Create_Tooltip() {

        }

        void Render(float x, float y) {

        }

    };










}