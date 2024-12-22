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
#include "textures.h"
#include "Text/text.h"
#include "Tooltips/tooltip.h"
#include "base_structs.h"

namespace  Skill {

    template <typename C>
    bool Increase_Skill(C &skill, int statRequirement) {
        if (skill.level < skill.maxLevel) {
            if (skill.level == 0 && statRequirement >= 30) {
                skill.level++;
                return true;
            }
            if (skill.level == 1 && statRequirement >= 60) {
                skill.level++;
                return true;
            }
            if (skill.level == 2 && statRequirement >= 90) {
                skill.level++;
                return true;
            }
        }
        return false;
    }

    struct Skill_Values {
        int unspent;
        int cost;
        int maxLevel;
        int statRequirement;
    };

    enum Tooltip_Line {
        UNTRAINED,
        BASIC,
        ADVANCED,
        EXPERT,
        NOTE
    };

    template <size_t T, typename  Action>
    class Skill_Tree {
        //display
        bool isOpen;
        int hoveredSkill;
        int hoveredSkillUp;
        static constexpr const char* maxLearnableLevel[3] = { "Basic", "Advanced", "Expert" };

        SDL_FRect scaledFrame{};
        SDL_Texture* texture;
        std::string name;

        std::array<std::string, T> names;
        std::array<std::array<std::string, 5>, T> descriptions;
        std::array<std::string, T> icons;

        //for mouseover
        float iconSize;
        std::array<SDL_FRect, T> skillHoverRects;
        std::array<SDL_FRect, T> skillUpRects;

    public:
        Skill_Tree(std::array<std::string, T> actions, std::array<std::string, T> iconKeys, std::array<std::array<std::string, 5>, T> descriptionList, std::string skillName) {
            name = std::move(skillName);
	    isOpen = false;
	    hoveredSkillUp = -1;
	    hoveredSkill = -1;
	    texture = nullptr;
	    iconSize = 36.0f;

            for (int i = 0; i < T; i++) {
                icons[i] = iconKeys[i];
                names[i] = actions[i];
                descriptions[i] = descriptionList[i];
            }
        }

        void Update(f2 scale, std::array<std::pair<int , int>, T> values,  Skill_Values skillPoints) {
	    SDL_Rect frame = {0, 0, 1137, 700};
            scaledFrame = UI::Center_Rect(frame);
            scaledFrame = UI::Update_Scale(scale, scaledFrame);
            FC_Scale FC_scale = {1.0f, 1.0f};

            //draw to texture
            if (texture) {
                SDL_DestroyTexture(texture);
                texture = nullptr;
            }

            texture = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frame.w, frame.h);
            SDL_SetRenderTarget(Graphics::renderer, texture);
            SDL_RenderCopyF(Graphics::renderer, Texture::spellbook, nullptr, nullptr);

            //Skill class name
            //cost per skill up
            //max level trainable for character
            //unspent skill points

            int n = ceil((float)T  * 0.5f);
            float ySpacing = 7.0f;
            float edge = 3.0f;

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

            x = 335.0f;
            y+= 30.0f;
            for (auto i : maxLearnableLevel) {
                FC_DrawScale_Center(Graphics::fcFont, Graphics::renderer, x, y, FC_scale, Color::black, i);
                x+= 70.0f;
            }

            SDL_FRect renderRect;

            for (int i = 0; i < n; i++) {
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
                float end;
                renderRect = {335.0f , 165.0f  + (i * (iconSize + ySpacing) + iconSize * 0.25f), (iconSize + ySpacing) * 3.0f, iconSize * 0.5f};
                end = renderRect.x + renderRect.w;
                //background
                clipRect = Icons::iconClipRects["maparch"].clipRect;
                clipRect.w = clipRect.w * (values[i].second / 3.0f);
                renderRect.w = renderRect.w * (values[i].second / 3.0f);
                SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &renderRect);

                for (int j = 0; j < values[i].second; ++j) {
                    //foreground
                    if (j < values[i].first) {
                        renderRect = {335.0f + (j * (iconSize + ySpacing)), 165.0f  + (i * (iconSize+ ySpacing) + iconSize * 0.25f), iconSize + ySpacing, iconSize * 0.5f};
                        clipRect = Icons::iconClipRects["experiencepoints"].clipRect;
                        SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &renderRect);
                    }
                }
                //increase button
                if ((skillPoints.unspent >= skillPoints.cost) && (values[i].first < values[i].second)) {
                    //icon
                    skillUpRects[i] = {end + 10.0f, 165.0f  + (i * (iconSize + ySpacing) + (iconSize * 0.125f)), iconSize * 0.75f, iconSize* 0.75f};
                    clipRect = Icons::iconClipRects["increasebutton"].clipRect;
                    SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &skillUpRects[i]);
                }

                skillUpRects[i] = UI::Update_Scale(scale, skillUpRects[i]);

                SDL_FRect gg = {128.0f - edge - 10.0f, (165.0f - edge)  + (i * (iconSize + ySpacing)), ( end - (128.0f - edge) + 20.0f), iconSize + (edge * 2.0f)};
                skillHoverRects[i] = UI::Update_Scale(scale, gg);
            }

            //skill name
            x = 700.0f;
            y = 115.0f;
            std::string availableSkillPoints = "Available Skill Points: " + std::to_string(skillPoints.unspent);
            FC_DrawScaleLeftColor(Graphics::fcFont, Graphics::renderer, x, y,  { 1.25f, 1.25f }, Color::black, availableSkillPoints.c_str());

            x = 845.0f;
            y += 30.0f;
            for (auto i : maxLearnableLevel) {
                FC_DrawScale_Center(Graphics::fcFont, Graphics::renderer, x, y, FC_scale, Color::black, i);
                x+= 70.0f;
            }

            for (int i = n; i < T; i++) {
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

                float end;
                renderRect = {850.0f , 165.0f  + ((i - n) * (iconSize + ySpacing) + iconSize * 0.25f), (iconSize + ySpacing) * 3.0f, iconSize * 0.5f};
                end = renderRect.x + renderRect.w;
                //background
                clipRect = Icons::iconClipRects["maparch"].clipRect;
                clipRect.w = clipRect.w * (values[i].second / 3.0f);
                renderRect.w = renderRect.w * (values[i].second / 3.0f);
                SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &renderRect);

                for (int j = 0; j < values[i].second; ++j) {
                    //foreground
                    if (j < values[i].first) {
                        renderRect = {850.0f + (j * (iconSize + ySpacing)), 165.0f  + ((i - n) * (iconSize+ ySpacing) + iconSize * 0.25f), iconSize + ySpacing, iconSize * 0.5f};
                        clipRect = Icons::iconClipRects["experiencepoints"].clipRect;
                        SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &renderRect);
                    }
                }

                //increase button
                if ((skillPoints.unspent >= skillPoints.cost) && (values[i].first < values[i].second)) {
                    //icon
                    skillUpRects[i] = {end + 10.0f, 165.0f  + ((i - n) * (iconSize + ySpacing) + (iconSize * 0.125f)), iconSize * 0.75f, iconSize* 0.75f};
                    clipRect = Icons::iconClipRects["increasebutton"].clipRect;
                    SDL_RenderCopyF(Graphics::renderer, Texture::cox_icons, &clipRect, &skillUpRects[i]);
                }

                skillUpRects[i] = UI::Update_Scale(scale, skillUpRects[i]);

                SDL_FRect gg = {620.0f - edge - 10.0f, (165.0f - edge)  + ((i - n) * (iconSize + ySpacing)), ( end - (620.0f - edge) + 20.0f ), iconSize + (edge * 2.0f)};
                skillHoverRects[i] = UI::Update_Scale(scale, gg);
            }

            SDL_SetRenderTarget(Graphics::renderer, nullptr);
        }

        int Click() {
            return hoveredSkillUp;
        }

    private:
        void Skill_Hover() {
            for (int i = 0; i < T; i++) {
                SDL_FRect rect = skillHoverRects[i];
                rect.x += scaledFrame.x;
                rect.y += scaledFrame.y;
                if (Mouse::FRect_inside_Screen_Cursor(rect)) {
                    hoveredSkill = i;
                    return;
                }
            }
            hoveredSkill = -1;
        }

        void Skill_Up_Hover() {
            for (int i = 0; i < T; i++) {
                SDL_FRect rect = skillUpRects[i];
                rect.x += scaledFrame.x;
                rect.y += scaledFrame.y;
                if (Mouse::FRect_inside_Screen_Cursor(rect)) {
                    hoveredSkillUp = i;
                    return;
                }
            }
            hoveredSkillUp = -1;
        }

	//needs to be lage enough to hold all text, then I need to just clip out the portion with the text and render it to the tooltip background texure
	void Draw_Tooltip(f2 scale, int i) {
            std::array<std::string, 6> formattedDesc = {
                    names[i],
                    "Untrained " + names[i] + descriptions[i][UNTRAINED],
                    "Basic " + names[i] + descriptions[i][BASIC],
                    "Advanced " + names[i] + descriptions[i][ADVANCED],
                    "Expert " + names[i] + descriptions[i][EXPERT],
                    descriptions[i][NOTE]
            };

            float lineHeight = FC_GetHeight(Graphics::fcFont, "%s", "A");
            std::array<float, 6> spacing = {
                    lineHeight * 0.50f,
                    lineHeight * 0.25f,
                    lineHeight * 0.25f,
                    lineHeight * 0.25f,
                    lineHeight * 0.50f,
                    lineHeight * 0.25f,
            };

            Tooltips::Properties<6> tooltipProperties = {
                    formattedDesc,
                    spacing,
		    550.0f,
                    20.0f,
                    10.0f,
                    Tooltips::MOUSE_TOP_RIGHT
            };

            Tooltips::Create_Tooltip(tooltipProperties);
        }

    public:
	bool Mouse_Inside() {
	    return Mouse::FRect_inside_Screen_Cursor(scaledFrame);
	}

	void Mouse_Over() {
	    if (!isOpen)
		return;

	    if (!Mouse_Inside())
		return;

	    Skill_Hover();
	    Skill_Up_Hover();
	}

	SDL_Texture* Draw(f2 scale) {
            //draw texture
            if (isOpen) {
		//render frame
                SDL_RenderCopyF(Graphics::renderer, texture, nullptr, &scaledFrame);

		//highlight hovered skill
		if (hoveredSkill >= 0 && hoveredSkill < T) {
                    SDL_FRect rect = skillHoverRects[hoveredSkill];
                    rect.x += scaledFrame.x;
                    rect.y += scaledFrame.y;
                    SDL_SetRenderDrawColor(Graphics::renderer, 20, 20, 20, 25);
                    SDL_RenderFillRectF(Graphics::renderer, &rect);

                    //tooltip
                    Draw_Tooltip(scale, hoveredSkill);
                }
		//highlight hovered skill up button
		else if (hoveredSkillUp >= 0 && hoveredSkillUp < T) {
                    SDL_FRect rect = skillUpRects[hoveredSkillUp];
                    rect.x += scaledFrame.x;
                    rect.y += scaledFrame.y;
                    SDL_SetRenderDrawColor(Graphics::renderer, 30, 10, 10, 25);
                    SDL_RenderFillRectF(Graphics::renderer, &rect);

                    //tooltip
                    Draw_Tooltip(scale, hoveredSkillUp);
                }
            }
	    return texture;
        }

	bool Toggle(Toggle_Type toggleType = Toggle_Type::toggle) {
            if (toggleType == Toggle_Type::get)
                return isOpen;

            if (toggleType == Toggle_Type::on) {
                isOpen = true;
                return true;
            }
            if (toggleType == Toggle_Type::off) {
                isOpen = false;
                hoveredSkillUp = -1;
                hoveredSkill = -1;
                return false;
            }

            if (isOpen) {
                isOpen = false;
                hoveredSkillUp = -1;
                hoveredSkill = -1;
                return false;
            }
            isOpen = true;
            return true;
        }

        void Close() {
            hoveredSkillUp = -1;
            hoveredSkill = -1;
            isOpen = false;
        }
    };
}