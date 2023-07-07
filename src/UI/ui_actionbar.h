#pragma once
#include "entt/entt.hpp"
#include "ui_elements.h"
#include "graphics.h"
#include "item_components.h"

namespace Action_Bar {
    struct On_Spellbar {

    };

    struct Spell_Bar {
        entt::entity spell[10];
        SDL_FRect actionBarFrame = { 0.0f, 0, 640.0f, 64.0f};
    };

    Spell_Bar actionBar;

    void Create_Action_Bar(entt::registry &zone) {
        // initialize value of empty
        entt::entity emptyBarSlot = zone.create();
        zone.emplace<Component::Renderable>(emptyBarSlot);
        zone.emplace<On_Spellbar>(emptyBarSlot);
        auto &icon = zone.emplace<Component::Icon>(emptyBarSlot, Graphics::emptyBagIcon, Graphics::default_icon, Item_Component::rarityBorder[Item_Component::Rarity::common], Graphics::bagSlotBorder);
        icon.clipSprite = {0,0,256,256};
        icon.clipIcon = {0,0,256,256};
        icon.renderRectSize = { 64.0f, 64.0f };
        icon.renderPositionOffset = { icon.renderRectSize.x / 2, icon.renderRectSize.y / 2 };

//    initialize action bar spell slots
        for (int i = 0; i < 10; i++) {
            actionBar.spell[i] = emptyBarSlot;
        }

        SDL_Rect rect = Utilities::SDL_FRect_To_SDL_Rect(actionBar.actionBarFrame);
        actionBar.actionBarFrame = UI::Center_Rect(rect);

        SDL_DisplayMode dm;
        SDL_GetWindowDisplayMode(Graphics::window, &dm);
        actionBar.actionBarFrame.y = dm.h - icon.renderRectSize.y;
    }


    void Render_Action_Bar(entt::registry &zone, Component::Camera &camera) {
        SDL_FRect renderBarFrame = UI::Update_Scale(camera.scale, actionBar.actionBarFrame);

        for (int i = 0; i < 10; i++) {
            auto &icon = zone.get<Component::Icon>(actionBar.spell[i]);
            SDL_FRect renderRect = {renderBarFrame.x + (renderBarFrame.h * i), renderBarFrame.y, renderBarFrame.h, renderBarFrame.h};

            SDL_RenderCopyF(Graphics::renderer, icon.pTexture, &icon.clipIcon, &renderRect);
        }
    }

}