#pragma once

#include "Graphics/graphics.h"
#include "SDL2/SDL.h"
#include "SQLite_insert.h"
#include "components.h"
#include "entt/entt.hpp"
#include "mouse_control.h"
#include "offsets.h"
#include "string"
#include "UI/Tooltips/tooltips.h"
#include "ui_elements.h"
#include "vector"

#include "offsets.h"
#include "radius.h"

namespace UI_Debug {

    struct Unit_Info {
        SDL_FRect panelRect;
        SDL_Rect backgroundFrame = {0, 0, 464, 571};
        SDL_Texture *background = nullptr;
    } spellbook;

    struct Data {
        std::string name;
        Rendering_Components::Sprite_Offset offset;
    };


    void Update_Position() {
        spellbook.panelRect = UI::Left_Rect(spellbook.backgroundFrame);
        spellbook.background = Texture::debug_window;
    }

    bool Mouse_Inside_Window(Component::Camera &camera) {
        SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
        if (Mouse::bRect_inside_Cursor(draw))
            return true;
        return false;
    }

    Data Get_Data(entt::registry &zone, entt::entity &entity) {
        auto name = zone.get<Component::Name>(entity).first;
        auto offset = zone.get<Rendering_Components::Sprite_Offset>(entity);
        return {name, offset};
    }

    void Draw_Frame(Component::Camera &camera, SDL_FRect &draw, const Data &data) {
        SDL_FRect renderRect = {draw.x + 192.0f, draw.y + 16.0f, 0.0f, 0.0f};
        SDL_FRect statBox = Camera_Control::Convert_FRect_To_Scale(renderRect, camera);
        FC_Scale scale = {1.0f / camera.scale.x, 1.0f / camera.scale.y};
        SDL_Color color = {255, 255, 255, 255};
        FC_DrawScaleRightColor(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, color, data.name.c_str());

        statBox.y += 30.0f / camera.scale.y;
        Label::Offsets(camera, statBox, scale, color, "offset");
        statBox.y += 30.0f / camera.scale.y;
        Label::Radius(camera, statBox, scale, color, "radius");
    }


    bool Update_Values(entt::registry &zone, Component::Camera &camera) {
        auto view = zone.view<Component::Selected, Component::Name, Component::Radius, Rendering_Components::Sprite_Offset>();
        for (auto entity: view) {
            auto &name = view.get<Component::Name>(entity);
            auto &offset = view.get<Rendering_Components::Sprite_Offset>(entity);
            auto &radius = view.get<Component::Radius>(entity);
            SDL_FRect panel = spellbook.panelRect;

            Button::Offsets(camera, panel, name.first, offset.x, offset.y);
            panel.y += 30.0f;
            Button::Radius(camera, panel, name.first, radius.fRadius, radius.fRadius);
        }
        return false;
    }

    bool open;

    void Draw(entt::registry &zone, Component::Camera &camera) {
        open = false;
        if (Debug::settings[Debug::debugFrame]) {
            auto view = zone.view<Component::Selected>();
            for (auto entity: view) {
                open = true;
                auto data = Get_Data(zone, entity);
                SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
                SDL_RenderCopyF(Graphics::renderer, spellbook.background, &spellbook.backgroundFrame, &draw);
                Draw_Frame(camera, spellbook.panelRect, data);
            }
        }
    }
}// namespace UI_Debug