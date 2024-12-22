//
// Created by nathanial on 12/18/24.
//
#pragma once
#include "base_structs.h"
#include "warrior.h"
#include "mage.h"
#include "rogue.h"
#include "general.h"
#include "skills.h"

namespace  Skill {


    void Update(entt::registry &zone, f2 scale) {
        Warrior::Update(zone, scale);
        Mage::Update(zone, scale);
        Rogue::Update(zone, scale);
        General::Update(zone, scale);
    }

    void Render(const f2 &scale) {
        General::Render(scale);
        Mage::Render(scale);
        Rogue::Render(scale);
        Warrior::Render(scale);
    }

    void HighLight() {
        General::Hover_Highlight();
        Mage::Hover_Highlight();
        Rogue::Hover_Highlight();
        Warrior::Hover_Highlight();
    }

    void Click(entt::registry &zone, entt::entity &entity,  int button) {
        bool clicked = false;
        switch (button) {
            case 5:
                clicked = General::Click(zone, entity);
                break;
            case 6:
                clicked = Mage::Click(zone, entity);
                break;
            case 7:
                clicked = Rogue::Click(zone, entity);
                break;
            case 8:
                clicked = Warrior::Click(zone, entity);
                break;
            default:
                break;
        }

        if (clicked)
            Update(zone, zone.get<Component::Camera>(entity).scale);
    }
}
