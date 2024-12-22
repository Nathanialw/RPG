//
// Created by desktop on 12/13/24.
//
#pragma once

#include "components.h"
#include "graphics.h"
#include "skills_Components.h"
#include "Toolbar/Skills/skill.h"
#include "Rendering/cox_rendering.h"

namespace XP {

    SDL_Rect XPBar;

    int Get_Level(int level) {
        int xp = 300;
        for (int i = 0; i < level - 1; i++)
            xp += (xp * 0.1);
        return xp;
    }

    void Get_XP(entt::registry &zone, int &state) {
        auto view = zone.view<Component::Level, Component::Input, Component::Stats, Component::Health, Skill_Component::Skills_Points, Component::Camera>();

        for (auto entity: view) {
            auto &xp = view.get<Component::Level>(entity);

            if (xp.xp >= Get_Level(xp.level)) {
                xp.xp = xp.xp  - Get_Level(xp.level);
                xp.level++;

                auto &stats = view.get<Component::Stats>(entity);
                stats.unspent += 10;

                auto &skills = view.get<Skill_Component::Skills_Points>(entity);
                skills.unspent += 1;


                skills.bankedFromInt += stats.intelligence;
                while (skills.bankedFromInt > 300) {
                    skills.unspent += 1;
                    skills.bankedFromInt -= 300;
                }

                auto &health = view.get<Component::Health>(entity);
                health.maxHealth += health.growth;
                health.base += health.growth;
                health.currentHealth = health.maxHealth;

                auto &camera = view.get<Component::Camera>(entity);
                Skill::Update(zone, camera.scale);

		COX_Render::levelUpGraphic.Set_Active();
            }
        }
    }

    void Render() {
        //+xp number floating up

        //level up graphic

        //level up sound

    }


    void Update(entt::registry &zone, int &state) {
        Get_XP(zone, state);
    }

}

