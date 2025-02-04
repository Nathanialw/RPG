#pragma once

#include "SDL2/SDL.h"
#include "Video/video.h"
//#include "ui.h"
#include "ui_actionbar.h"
#include "ui_elements.h"
#include "ui_spellbook.h"
#include "utilities.h"
#include <map>
#include "colors.h"

namespace Menu_Options {

    struct Option_Frame {
        UI::Text_Frame text;
        UI::Image_Frame checkBox;
    };

    struct Menu_Options_Frame {
        UI::Image_Frame backgroundFrame;
        std::array<Option_Frame, Debug::Settings::SIZE> List;
        std::array<const char *, Debug::Settings::SIZE> labels = {"Framerate", "Num Rendered", "Interaction Rects", "entity count", "Render Checks", "Collision Checks", "Num w/ Render Components", "Update Quad Tree Debug", "Quad Tree Size", "Entity Positions", "Loop Timers", "Lock Framerate",
                                                                  "Debug Frame", "Volume", "Show Pathing"};
    } Options;


    void Update_Position() {
        SDL_Rect rect = Utilities::SDL_FRect_To_SDL_Rect(Options.backgroundFrame.defaultFrame);
        Options.backgroundFrame.frame = UI::Center_Rect(rect);
        int w;
        int h;
        SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
        Options.backgroundFrame.frame.y = (float) h / 4.0f;
    }

    void Load_Options() {
        //    load from file
        Options.backgroundFrame.frame = {0.0f, 0.0f, 512.0f, 512.0f};
        Options.backgroundFrame.defaultFrame = {0.0f, 0.0f, 512.0f, 512.0f};
        Options.backgroundFrame.texture = Texture::tooltipBackground;
        Update_Position();
        //    asd
        for (int i = 0; i < Options.labels.size(); ++i) {
            Options.List[i].text.frame = {Options.backgroundFrame.frame.x + 16.0f, Options.backgroundFrame.frame.y + (i * 32.0f) + 16.0f, 64.0f, 32.0f};
            Options.List[i].text.text = "";
            Options.List[i].text.backgroundTexture = Texture::tooltipBackground;
            Options.List[i].text.textFrame = {Options.backgroundFrame.frame.x + 16.0f, Options.backgroundFrame.frame.y + (i * 32.0f) + 16.0f, strlen(Options.labels[i]) * 15.0f, 32.0f};
            Options.List[i].text.textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, TTF_RenderText_Solid(Graphics::font, Options.labels[i], Color::white));

            Options.List[i].checkBox.frame = {Options.backgroundFrame.frame.x + Options.backgroundFrame.frame.w - 64.0f, Options.backgroundFrame.frame.y + (i * 32.0f) + 16.0f, 32.0f, 32.0f};
            Options.List[i].checkBox.defaultFrame = {Options.backgroundFrame.frame.x + Options.backgroundFrame.frame.w - 64.0f, Options.backgroundFrame.frame.y + (i * 32.0f) + 16.0f, 32.0f, 32.0f};
            Options.List[i].checkBox.texture = Texture::tooltipBackground;
        }
    }

    int Show_Menu_Options(Component::Camera &camera) {
        Video::Update_Volume();
        Update_Position();
        SDL_FRect renderOptions = UI::Update_Scale(camera.scale, Options.backgroundFrame.frame);
        SDL_RenderCopyF(Graphics::renderer, Options.backgroundFrame.texture, nullptr, &renderOptions);

        int i = 0;
        for (auto &option: Options.List) {
            SDL_FRect text = UI::Update_Scale(camera.scale, option.text.textFrame);
            SDL_RenderCopyF(Graphics::renderer, option.text.textTexture, nullptr, &text);

            option.checkBox.frame = UI::Update_Scale(camera.scale, option.checkBox.defaultFrame);
            SDL_RenderCopyF(Graphics::renderer, option.checkBox.texture, nullptr, &option.checkBox.frame);
            (Debug::settings[i]) ? option.checkBox.texture = Texture::checkmark : option.checkBox.texture = Texture::tooltipBackground;
            i++;
        }

        while (SDL_PollEvent(&Events::event) != 0) {
            switch (Events::event.type) {
                case SDL_WINDOWEVENT: {
                    if (Events::event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        //            recenter camera on player
                        UI_Spellbook::Update_Position();
                        Action_Bar::Update_Position(Action_Bar::actionBar.actionBar.actionBarFrame);
                        Load_Options();
                    }
                }

                case SDL_MOUSEBUTTONUP: {
                    if (Events::event.button.type == SDL_MOUSEBUTTONUP) {
                        for (int i = 0; i < Options.labels.size(); ++i) {
                            if (Mouse::FRect_inside_Screen_Cursor(Options.List[i].checkBox.frame)) {
                                if (Debug::settings[i]) {
                                    Debug::settings[i] = false;
                                    //                  set texture to unchecked box
                                } else {
                                    Debug::settings[i] = true;
                                    //                  set texture to checked box
                                }
                                break;
                            }
                        }
                        break;
                    }
                }

                case SDL_KEYDOWN: {
                    if (Events::event.key.keysym.sym == SDLK_ESCAPE) {
                        return -1;
                    }
                    break;
                }
            }
        }
        return 4;
    }
}// namespace Menu_Options