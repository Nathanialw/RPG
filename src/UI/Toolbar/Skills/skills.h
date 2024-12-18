//
// Created by nathanial on 12/17/24.
//
#pragma  once
#include <utility>
#include <tuple>

#include "array"
#include "string"
#include "SDL2/SDL.h"
#include "graphics.h"
#include "textures.h"
#include "icons.h"
#include "Button_Bar/highlight.h"
#include "ui_elements.h"
#include "skills_Components.h"

namespace  Skills {

    struct Skill_Values {
        int unspent;
        int cost;
        int maxLevel;
    };

    //render book background
    //cost per skillup
    //max level trainable for character
    //unspent skill points

    //each skill
    //icon
    //name
    //text description
    //current level / max level

    template <size_t T, typename  Action>
    class Skill_Tree {
        //display
        bool toggle = false;
        SDL_Texture* background = nullptr;
        static constexpr const char* maxLearnableLevel[3] = { "Basic", "Advanced", "Expert" };

        SDL_FRect frame = {0, 0, 1137, 700};
        SDL_FRect scaledFrame{};
        SDL_Texture* texture = nullptr;
        std::string name;

        std::array<std::string, T> names;
        std::array<std::string, T> descriptions;
        std::array<std::string, T> icons;

        //for mouseover
        float iconSize = 36.0f;
        std::array<SDL_FRect, T> skillHoverRects;
        std::array<SDL_FRect, T> skillUpRects;

    public:
        Skill_Tree(std::array<std::string, T> actions, std::array<std::string, T> iconKeys, std::string skillName) {
            name = std::move(skillName);

            for (int i = 0; i < T; i++) {
                icons[i] = iconKeys[i];
                names[i] = actions[i];
                descriptions[i] = actions[i] + std::to_string(i) + " description";
            }
        }

        void Update(f2 scale, std::array<std::pair<int , int>, T> values,  Skill_Values skillPoints) { //tuple is {unspent, cost, max level}
            scaledFrame = UI::Center_Rect(Utilities::SDL_FRect_To_SDL_Rect(frame));
            scaledFrame = UI::Update_Scale(scale, scaledFrame);
            FC_Scale FC_scale = {1.0f, 1.0f};

            //draw to texture
            if (texture) {
                SDL_DestroyTexture(texture);
                texture = nullptr;
            }
            if (!background)
                background = Texture::spellbook;

            texture = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frame.w, frame.h);
            SDL_SetRenderTarget(Graphics::renderer, texture);
            SDL_RenderCopyF(Graphics::renderer, background, nullptr, nullptr);

            //Skill class name
            //cost per skill up
            //max level trainable for character
            //unspent skill points

            int n = ceil((float)T  * 0.5f);
            float ySpacing = 7.0f;
            float edge = 3.0f;

            for (int i = 0; i < n; i++) {
                //skill name
                float x = 175.0f;
                float y = 75.0f;
                std::string skillName = name  + " Skills";
                FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, x, y, FC_scale, Color::black, skillName.c_str());

                y+= 20.0f;
                std::string cost = "Cost: " + std::to_string(skillPoints.cost);
                FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, x, y, FC_scale, Color::black, cost.c_str());

                y+= 20.0f;
                std::string maxTrainable ="Max Trainable Level: ";
                maxTrainable.append(maxLearnableLevel[skillPoints.maxLevel]);
                FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, x, y, FC_scale, Color::black, maxTrainable.c_str());

                //icon background
                skillUpRects[i] = {128.0f - edge, (165.0f - edge)  + (i * (iconSize + ySpacing)), iconSize + (edge * 2.0f), iconSize + (edge * 2.0f)};
                SDL_Rect clipRect = Icons::iconClipRects["black square"].clipRect;
                SDL_RenderCopyF(Graphics::renderer, Texture::cox_objects, &clipRect, &skillUpRects[i]);

                //icon
                skillUpRects[i] = {128.0f, 165.0f  + (i * (iconSize + ySpacing)), iconSize, iconSize};
                clipRect = Icons::iconClipRects[icons[i]].clipRect;
                SDL_RenderCopyF(Graphics::renderer, Texture::cox_skills, &clipRect, &skillUpRects[i]);

                //name
                x = 128.0f + iconSize + 10.0f;
                y = 165.0f + (i * (iconSize + ySpacing)) + (iconSize * 0.25f);
                FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, x, y, FC_scale, Color::black, names[i].c_str());

                //current level / max level
                x = 450.0f;
                std::string value = std::to_string(values[i].first) + " / " + std::to_string(values[i].second);
                FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, x, y, FC_scale, Color::black, value.c_str());

                //increase button
                if (skillPoints.unspent > 0) {
//                icons["increasebutton";
                }
            }

            for (int i = n; i < T; i++) {
                //skill name
                float x = 700.0f;
                float y = 100.0f;
                std::string availableSkillPoints = "Available Skill Points: " + std::to_string(skillPoints.unspent);
                FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, x, y,  { 1.25f, 1.25f }, Color::black, availableSkillPoints.c_str());

                //icon background
                skillUpRects[i] = {620.0f - edge, (165.0f - edge)  + ((i - n) * (iconSize + ySpacing)), iconSize + (edge * 2.0f), iconSize + (edge * 2.0f)};
                SDL_Rect clipRect = Icons::iconClipRects["black square"].clipRect;
                SDL_RenderCopyF(Graphics::renderer, Texture::cox_objects, &clipRect, &skillUpRects[i]);

                //icon
                skillUpRects[i] = {620.0f, 165.0f  + ((i - n) * (iconSize + ySpacing)), iconSize, iconSize};
                clipRect = Icons::iconClipRects[icons[i]].clipRect;
                SDL_RenderCopyF(Graphics::renderer, Texture::cox_skills, &clipRect, &skillUpRects[i]);

                //name
                x = 620.0f + iconSize + 10.0f;
                y = 165.0f + ((i - n) * (iconSize + ySpacing)) + (iconSize * 0.25f);
                FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, x, y, FC_scale, Color::black, names[i].c_str());

                //current level / max level
                x = 950.0f;
                std::string value = std::to_string(values[i].first) + " / " + std::to_string(values[i].second);
                FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, x, y, FC_scale, Color::black, value.c_str());

                //increase button
                if (skillPoints.unspent > 0) {
//                icons["increasebutton";
                }
            }

            SDL_SetRenderTarget(Graphics::renderer, nullptr);
        }

        void Mouse_Over() {
            //draw tooltip
        }

        void Draw() {
            //draw texture
            if (toggle)
                SDL_RenderCopyF(Graphics::renderer, texture, nullptr, &scaledFrame);
        }

        int Skill_Up() {
            return 1;
        }

        bool Toggle() {
            if (toggle) {
                toggle = false;
                return false;
            }
            toggle = true;
            return true;
        }

        void Close() {
            toggle = false;
        }
    };
}