//
// Created by nathanial on 12/19/24.
//
#pragma once

#include <span>
#include "string"
#include "SDL2/SDL.h"
#include "graphics.h"
#include "tooltips.h"
#include "textures.h"

namespace Text {

    struct Texture_Data {
    private:
        FC_Scale scale{};
        SDL_FRect textFrame{};
        float padding{};
        float fc_scale = 2.0f;

    public:
        SDL_FRect frame{};
        SDL_Texture *texture = nullptr;

        //create a multiline text texture
        Texture_Data(const std::vector<std::string> &lines, float w, float h, float pad, float fc_scale = 1.0f) {
            float  lineHeight = h;
            h = (lines.size() * lineHeight);
            frame = {0.0f, 0.0f, w + (pad * 2.0f),  h + (pad * 2.0f)};

            padding = pad * fc_scale ;
            scale = {fc_scale, fc_scale};
            textFrame = {0.0f, 0.0f, (w * scale.x) + (padding * 2.0f), (h * scale.y) + (padding * 2.0f)};
            texture = Graphics::Create_Canvas((int)textFrame.w, (int)textFrame.h);

            //save scale
            float x, y;
            SDL_RenderGetScale(Graphics::renderer, &x, &y);

            //set target
            SDL_SetRenderTarget(Graphics::renderer, texture);
            //set scale
            SDL_RenderSetScale(Graphics::renderer, fc_scale, fc_scale);

            //background
            Graphics::Render_FRect(Texture::tooltipBackground, Color::white, nullptr, nullptr);

            //draw text
            textFrame.y += padding * 0.5f;
            FC_Scale FC_scale = {1.0f, 1.0f};
            for (const auto &line: lines) {
                if (line.empty())
                    continue;

                FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, textFrame.x + padding * 0.5f, textFrame.y, FC_scale, Color::white, line.c_str());
                textFrame.y += (lineHeight);
            }

            //reset target
            SDL_SetRenderTarget(Graphics::renderer, nullptr);
        }

        //Skill tooltips
        Texture_Data(const std::string &title, const std::span<std::string> lines, float w) {
            float lineHeight = FC_GetHeight(Graphics::fcFont, "%s", lines[0].c_str());
            scale = {1.0f, 1.0f};
            frame = {0.0f, 0.0f, 0.0f, 0.0f};

            //draw title
            FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, frame.x, frame.y, scale, Color::white, title.c_str());
            frame.y += lineHeight;
            frame.y += lineHeight * 0.25f;

            //draw body
            for (int j = 0; j < lines.size(); j++) {
                if (lines[j].empty())
                    continue;

                std::vector<std::string> formattedLines;
                frame.w = FC_GetWidth(Graphics::fcFont, "%s", lines[j].c_str());

                if (frame.w <= w) {
                    formattedLines.push_back(lines[j]);
                }
                else {
                    int charCount = 100;

                    for (int i = 0; i < lines[j].size(); i += charCount) {
                        //grab a line
                        std::string formattedLine = lines[j].substr(i, charCount);

                        //get a substring to the last space
                        if (lines[j].substr(lines[j].size() - 1, 1) != " " ) {
                            int space = lines[j].find_last_of(' ');
                            formattedLine = lines[j].substr(i, space);
                            float lineW =FC_GetWidth(Graphics::fcFont, "%s", formattedLine.c_str());
                            while (lineW > w) {
                                formattedLine = formattedLine.substr(0, formattedLine.size() - 1);
                                space = formattedLine.find_last_of(' ');
                                formattedLine = formattedLine.substr(0, space);
                                lineW =FC_GetWidth(Graphics::fcFont, "%s", formattedLine.c_str());
                            }
                            i -= (charCount - space);
                        }
                        formattedLines.push_back(formattedLine);
                    }
                }

                int size = lines.size() - 1;
                if (j == size)  {
                    frame.y += lineHeight;
                }
                else {
                    frame.y += lineHeight * 0.25f;
                }

                for (const auto &renderLine: formattedLines) {
                    FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, frame.x, frame.y, scale, Color::white, renderLine.c_str());
                    frame.y += lineHeight;
                }
            }
        }

        ~Texture_Data() {
            if (texture)
                SDL_DestroyTexture(texture);
        }
    };


    f2 Get_Multiline_Rect(std::string &text, const float maxLineLength = 500.0f) {
        float lineW = FC_GetWidth(Graphics::fcFont, "%s", text.c_str());
        float h = FC_GetHeight(Graphics::fcFont, "%s", text.c_str());
        float w = 0.0f;

        //clean the end of the string before evaluating
        if (text.substr(text.size() - 1, 1) == " ")
            text = text.substr(0, text.size() - 1);

        //turn each string line into a multiline
        if (lineW < maxLineLength) {
            w = FC_GetWidth(Graphics::fcFont, "%s", text.c_str());
        }
        else {
            int charCount = 100;
            int n = 0;
            for (int i = 0; i < text.size(); i += charCount) {
                //grab a line
                std::string formattedLine = text.substr(i, charCount);

                //get a substring to the last space
                if (text.substr(text.size() - 1, 1) != " " ) {
                    int space = text.find_last_of(' ');
                    formattedLine =text.substr(i, space);
                    lineW =FC_GetWidth(Graphics::fcFont, "%s", formattedLine.c_str());
                    while (lineW > maxLineLength) {
                        formattedLine = formattedLine.substr(0, formattedLine.size() - 1);
                        space = formattedLine.find_last_of(' ');
                        formattedLine = formattedLine.substr(0, space);
                        lineW =FC_GetWidth(Graphics::fcFont, "%s", formattedLine.c_str());
                    }
                    i -= (charCount - space);
                }
                float  textW = FC_GetWidth(Graphics::fcFont, "%s", formattedLine.c_str());
                if (w < textW)
                    w = textW;
                n++;
            }
            h = (h * n);
        }

        //use that to get the w and h of the text box
        return {w, h};
    }

    //create a multiline text texture
    void Create_Multiline_Texture(f2 scale, const std::string &text, float x, float y, int length = 100) {
        float w = 0.0f;
        float h = FC_GetHeight(Graphics::fcFont, "%s", text.c_str());

        std::vector<std::string> lines;

        if (text.size() < length) {
            lines.push_back(text);
            w = FC_GetWidth(Graphics::fcFont, "%s", text.c_str());
        }
        else {
            int charCount = length;
            for (size_t i = 0; i < text.size(); i += charCount) {
                auto line = text.substr(i, charCount);
                if (line.substr(line.size() - 1, 1) != " " && i + charCount < text.size()) {
                    auto space = line.find_last_of(' ');
                    line = line.substr(0, space);
                    i -= (charCount - space);
                }
                float  textW = FC_GetWidth(Graphics::fcFont, "%s", line.c_str());
                if (w < textW)
                    w = textW;
                lines.push_back(line);
            }
        }

        Texture_Data textureData = Texture_Data(lines, w, h, 7.5f);

        SDL_FRect tooltipPosition = {
                x,
                y - (textureData.frame.h / scale.y),
                textureData.frame.w,
                textureData.frame.h
        };

//        SDL_RenderCopyF(Graphics::renderer, textureData.texture, nullptr, &tooltipPosition);

        tooltipPosition = UI::Update_Scale_Size(scale, tooltipPosition);
        SDL_RenderCopyF(Graphics::renderer, textureData.texture, nullptr, &tooltipPosition);
    }
}