//
// Created by desktop on 12/13/24.
//
#pragma once

#include "components.h"
#include "graphics.h"

namespace XP {

    SDL_Rect XPBar;

    int Get_Level(int level) {
        int xp = 300;
        for (int i = 0; i < level; i++)
            xp += (xp * 0.1);
        return xp;
    }

    void Get_XP(entt::registry &zone, int &state) {
        auto view = zone.view<Component::XP, Component::Input>();

        for (auto entity: view) {
            auto &xp = view.get<Component::XP>(entity);


            XPBar = {0, 0, 5, 500};
        }
    }




    void Render(entt::registry &zone, int &state) {
        Get_XP(zone, state);

    }

}

